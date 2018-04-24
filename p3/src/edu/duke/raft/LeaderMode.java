package edu.duke.raft;

import java.util.Timer;
import java.util.*;

public class LeaderMode extends RaftMode {
	private Timer heartbeatTimer = new Timer();
	private int HEARTBEAT_TIMER_ID = 4;
	private Map <Integer,Integer> nextIndexMap = new HashMap<Integer,Integer>();

	public void go() {
		synchronized (mLock) {
			
			int term = mConfig.getCurrentTerm();
			
			System.out.println("S" + mID + "." + term + ": switched to leader mode.");

			// set term 
			RaftResponses.setTerm(term);
			RaftResponses.clearAppendResponses(mConfig.getCurrentTerm());
			RaftResponses.clearVotes(term);

			this.populateNextLogIndex();
			// send heartbeats
			this.sendHeartbeats();

			
			// schedule heartBeatTimer
			heartbeatTimer = this.scheduleTimer(HEARTBEAT_INTERVAL, this.HEARTBEAT_TIMER_ID);
			

		}
	}

	private void populateNextLogIndex(){
		int lastIndex = mLog.getLastIndex();
		for(int i=1; i <= mConfig.getNumServers();i++){
			nextIndexMap.put(i,lastIndex+1);
		}
	}

	// helper function to send heartbeat
	private void sendHeartbeats() {
		// int repairFailed = -1;
		int response = -1;

		// loop through the servers and send them the entry
		for(int i = 1; i <= mConfig.getNumServers();i++) {
			if(i !=mID){
				List <Entry> entries = new ArrayList<Entry>();


				int startIndex = nextIndexMap.get(i);
				int endIndex = mLog.getLastIndex();

				System.out.println( " Start " + startIndex + " End " + endIndex);


				if(startIndex <= endIndex){
					int entriesToSend = endIndex - startIndex+1;

					for(int k = 0; k < entriesToSend;k++){
						System.out.println(" log entry " + mLog.getEntry(nextIndexMap.get(k + startIndex)));
						entries.add(mLog.getEntry(nextIndexMap.get(k + startIndex)));
					}		
				}
				int prevLogTerm = mLog.getEntry(startIndex-1).term;

				Entry[] entriesArray = entries.toArray(new Entry[entries.size()]);

				System.out.println(" entries  " + Arrays.toString( entries.toArray()));
				System.out.println("prev log Term " + prevLogTerm);
			
				remoteAppendEntries (i, 
					mConfig.getCurrentTerm(),
					mID,
					(startIndex -1 ), // prevLogIndex
					prevLogTerm, // prevLogTerm
					entriesArray,mCommitIndex);
			}
		}
		System.out.println(" Completed leader sendHeartbeats");

	}
			

	private int getResponse(){
		int currentTerm = mConfig.getCurrentTerm();
		int[] appendResponses = RaftResponses.getAppendResponses(currentTerm);
		
		for(int i =1; i < mConfig.getNumServers();i++){
			if(i !=mID){
				if(appendResponses[i] == 0){
					nextIndexMap.put(i,mLog.getLastIndex()+1);
					
				}else if (appendResponses[i] > 0){
					int prevLogIndex = nextIndexMap.get(i);
					// decrease index term
					nextIndexMap.put(i,prevLogIndex -1);

					if(appendResponses[i] > currentTerm){
						currentTerm = appendResponses[i];
					}
				}
			}
		}

			RaftResponses.clearAppendResponses(currentTerm);
			return currentTerm;
	}


	



	// @param candidate’s term
	// @param candidate requesting vote
	// @param index of candidate’s last log entry
	// @param term of candidate’s last log entry
	// @return 0, if server votes for candidate; otherwise, server's
	// current term
	public int requestVote(int candidateTerm, int candidateID, int lastLogIndex, int lastLogTerm) {
		synchronized (mLock) {
			int term = mConfig.getCurrentTerm();
			int vote = 0;

			// System.out.println(" candidate " + candidateID + " term  " + candidateTerm);
			// reject vote
			if(candidateTerm <= term) vote= term;
			
			// if candidate term is higher than current term 
			if(candidateTerm > term ) {
				// candidate with more recent log - cancel timer, change to follower mode and vote 
				if( lastLogTerm >= mLog.getLastTerm() ) {
					this.heartbeatTimer.cancel();
					mConfig.setCurrentTerm(candidateTerm, candidateID);
					// System.out.println(" server " + mID + " changed to follower from leader ");
					// System.out.println(" lastLogTerm " + lastLogTerm + " mLog.getLastTerm " + mLog.getLastTerm());
				
					RaftServerImpl.setMode(new FollowerMode());
					vote= 0;
				}else { // candidate has the older log - dont vote
					this.heartbeatTimer.cancel();
					mConfig.setCurrentTerm(candidateTerm,0);
					RaftServerImpl.setMode(new FollowerMode());
					vote = term;
				}
			}
			
			return vote;

		}
	}

	// @param leader’s term
	// @param current leader
	// @param index of log entry before entries to append
	// @param term of log entry before entries to append
	// @param entries to append (in order of 0 to append.length-1)
	// @param index of highest committed entry
	// @return 0, if server appended entries; otherwise, server's
	// current term
	public int appendEntries(int leaderTerm, int leaderID, int prevLogIndex, int prevLogTerm, Entry[] entries,
			int leaderCommit) {
		synchronized (mLock) {
			int term = mConfig.getCurrentTerm();
			int result = term;
			// System.out.println(" leader appendEntries " + mID + " " + term + " " + leaderTerm);
			//Doing it just to be safe. Don't think a leader would replicate on itself.
			if(leaderTerm>term){
			    this.heartbeatTimer.cancel();
			    RaftResponses.setTerm(leaderTerm);
			    RaftServerImpl.setMode(new FollowerMode());

			    // System.out.println(" leaderTerm is greater " + mID);
			    return 0;
			}
			if (mID==leaderID) {
				return 0;
			}
			
			return result;
		}
	}

	// @param id of the timer that timed out
	public void handleTimeout(int timerID) {
		synchronized (mLock) {
			// heartbeat timer expired
			if(timerID == this.HEARTBEAT_TIMER_ID) {
				// restart timer
				int response = this.getResponse();

				if(response > mConfig.getCurrentTerm()){

					// Convert to follower
					this.heartbeatTimer.cancel();
					RaftServerImpl.setMode(new FollowerMode());
				}
				
				// send heartbeats
			    this.sendHeartbeats();
			    // reset timer
			    this.heartbeatTimer.cancel();
				// reschedule heartBeatTimer
				heartbeatTimer = this.scheduleTimer(HEARTBEAT_INTERVAL, this.HEARTBEAT_TIMER_ID);

			}
		}
	}
}