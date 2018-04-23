package edu.duke.raft;

import java.util.Timer;
import java.util.*;

public class LeaderMode extends RaftMode {
	private Timer heartbeatTimer = new Timer();
	private int HEARTBEAT_TIMER_ID = 4;
	private List<Integer> nextLogIndex = new ArrayList<Integer>();

	public void go() {
		synchronized (mLock) {
			
			int term = mConfig.getCurrentTerm();
			System.out.println("S" + mID + "." + term + ": switched to leader mode.");
			
<<<<<<< HEAD
			//Set the term.
			
	
=======
			this.populateNextLogIndex();
>>>>>>> 061da1c9065bf1c98d750337d619a21b10565dd9
			// send heartbeats
			this.sendHeartbeats();

			
			// schedule heartBeatTimer
			heartbeatTimer = this.scheduleTimer(HEARTBEAT_INTERVAL, this.HEARTBEAT_TIMER_ID);

		}
	}

	private void populateNextLogIndex(){
		for(int i=1; i <= mConfig.getNumServers();i++){
			nextLogIndex.add(mLog.getLastIndex()+1);
		}
	}

	// helper function to send heartbeat
	private void sendHeartbeats() {
<<<<<<< HEAD
		RaftResponses.setTerm(mConfig.getCurrentTerm());
		int lastMatchedIndex = mLog.getLastIndex();
		int lastMatchedIndexPointer = lastMatchedIndex;
		int repairFailed = -1;
			
=======
		int term = mConfig.getCurrentTerm();
		//Set the term.
		RaftResponses.setTerm(term);
		RaftResponses.clearAppendResponses(mConfig.getCurrentTerm());

		
		// int repairFailed = -1;
		int response = -1;

>>>>>>> 061da1c9065bf1c98d750337d619a21b10565dd9
		// loop through the servers and send them the entry
		for(int i = 1; i <= mConfig.getNumServers();i++) {
			List <Entry> entries = new ArrayList<Entry>();

			for(int j =nextLogIndex.get(i); j <=mLog.getLastIndex();j++){
				entries.add(mLog.getEntry(nextLogIndex.get(j)));
			}	
			

			Entry[] entriesArray = entries.toArray(new Entry[entries.size()]);
			
			int prevLogTerm = mLog.getLastTerm();
	
			remoteAppendEntries (i, mConfig.getCurrentTerm(),mID,
					(nextLogIndex.get(i)-1),prevLogTerm,entriesArray,mCommitIndex);
			
			}
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
			// reject vote
			if(candidateTerm <= term) vote= term;
			
			// if candidate term is higher than current term 
			if(candidateTerm > term ) {
				// candidate with more recent log - cancel timer, change to follower mode and vote 
				if( lastLogTerm >= mLog.getLastTerm() ) {
					this.heartbeatTimer.cancel();
					mConfig.setCurrentTerm(candidateTerm, candidateID);
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
			    RaftServerImpl.setMode(new FollowerMode());
			    // System.out.println(" leaderTerm is greater " + mID);
			    return 0;
			}
			if (leaderTerm==term) {
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
				this.heartbeatTimer.cancel();
				// reschedule heartBeatTimer
				heartbeatTimer = this.scheduleTimer(HEARTBEAT_INTERVAL, this.HEARTBEAT_TIMER_ID);
				
				// send heartbeats
			    this.sendHeartbeats();
			}
		}
	}
}
