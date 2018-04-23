package edu.duke.raft;

import java.util.Random;
import java.util.Timer;

public class CandidateMode extends RaftMode {
//	private Timer checkVoteTimer;
	private Timer electionTimeoutTimer;
//	private int CHECKVOTE_TIMER_ID = 1;
	private int ELECTION_TIMER_ID = 2;
//	private int CHECK_VOTES_DURATION;
	private int electionTimeoutDuration;

	public void go() {
		
		synchronized (mLock) {
			// increase current term
			int currentTerm = mConfig.getCurrentTerm();
			currentTerm += 1;
			mConfig.setCurrentTerm(currentTerm, 0); // ? votedFor as mID

			// set term
			RaftResponses.setTerm(mConfig.getCurrentTerm());
			// clear votes for the current term
			RaftResponses.clearVotes(currentTerm); //EDIT ---- Chase says Maybe remove this? 
			
			// schedule electiontimer
			Random random = new Random();
			electionTimeoutDuration = random.nextInt(ELECTION_TIMEOUT_MAX - ELECTION_TIMEOUT_MIN)
					+ ELECTION_TIMEOUT_MIN;
//			this.ELECTION_TIMER_ID = mID * 7;
			this.electionTimeoutTimer = scheduleTimer(electionTimeoutDuration, ELECTION_TIMER_ID);

			// schedule checkVotes timer
//			this.CHECKVOTE_TIMER_ID = mID * 3;
//			this.CHECK_VOTES_DURATION = 120;
//			this.checkVoteTimer = scheduleTimer(CHECK_VOTES_DURATION, CHECKVOTE_TIMER_ID);

			// request votes from all other servers
			for (int i = 1; i <= mConfig.getNumServers(); i++) {
				this.remoteRequestVote(i, mConfig.getCurrentTerm(), mID, mLastApplied, mLog.getLastTerm());
			}
			System.out.println("S" + mID + "." + currentTerm + ": switched to candidate mode.");
		}

	}

	// have a checkVote time out function , array
	// count then - cancel both timers before you set mode to leader

	// @param candidate’s term
	// @param candidate requesting vote
	// @param index of candidate’s last log entry
	// @param term of candidate’s last log entry
	// @return 0, if server votes for candidate; otherwise, server's
	// current term
	public int requestVote(int candidateTerm, int candidateID, int lastLogIndex, int lastLogTerm) {
		synchronized (mLock) {
			int term = mConfig.getCurrentTerm();
			// vote for someone who is requesting a vote with a higher term
			if(candidateTerm > term) {
				//Checking if up to date before voting for them.
				if (lastLogTerm>mLog.getLastTerm()||
					( lastLogTerm==mLog.getLastTerm() && lastLogIndex>=mLog.getLastIndex()) ) {
					//Change term and vote for that guy with a higher term.
					this.electionTimeoutTimer.cancel();

					mConfig.setCurrentTerm(candidateTerm, candidateID);
					RaftServerImpl.setMode(new FollowerMode());
					return 0;
					
				}else{
					//this.electionTimeoutTimer.cancel();
					mConfig.setCurrentTerm(candidateTerm, 0);
					return term;
				}
				
			}else if (mID == candidateID) {
				return 0;
			}else {
				return term;
			}
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

			// Reset timer
			this.electionTimeoutTimer.cancel();
			Random random = new Random();
			electionTimeoutDuration = random.nextInt(ELECTION_TIMEOUT_MAX - ELECTION_TIMEOUT_MIN)
					+ ELECTION_TIMEOUT_MIN;
			this.electionTimeoutTimer = scheduleTimer(electionTimeoutDuration, ELECTION_TIMER_ID);

			
			// System.out.println(" candidate append Entries " + mID);
			int term = mConfig.getCurrentTerm();
			
			// check for leaderMode
			if (leaderTerm >= term) {
				this.electionTimeoutTimer.cancel();
				mConfig.setCurrentTerm(leaderTerm, 0);
				RaftServerImpl.setMode(new FollowerMode());
				return 0;
			}
			return term;
		}
	}

	// @param id of the timer that timed out
	public void handleTimeout(int timerID) {
		synchronized (mLock) {
			
			// Count votes- check votes timeout
				int[] votes = RaftResponses.getVotes(mConfig.getCurrentTerm());
				int totalVote =0;
				
				// check if you got any votes at all 
				if(votes!=null) {
					for(int i =0; i <votes.length;i++ ) {
						totalVote = (votes[i]==0) ? totalVote+1:totalVote;
					}
					// If won the election
					if(totalVote > mConfig.getNumServers()/2) {
						this.electionTimeoutTimer.cancel();
					//System.out.println(" Server " + mID + " got " + totalVote+ " votes ");
						RaftServerImpl.setMode(new LeaderMode());
						
					}else {
						this.electionTimeoutTimer.cancel();
						go();
					}
				
				}else {
					this.electionTimeoutTimer.cancel();
					go();
				}
		}
	}
}
