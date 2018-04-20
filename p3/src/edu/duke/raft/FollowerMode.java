package edu.duke.raft;

import java.util.Random;
import java.util.Timer;

public class FollowerMode extends RaftMode {
	private int TIMER_ID = 3;
	private Timer timer;

	public void go() {

		synchronized (mLock) {
			// schedule timer
			Random random = new Random();
			int electionTimeOut = random.nextInt(ELECTION_TIMEOUT_MIN - ELECTION_TIMEOUT_MAX) + 
					ELECTION_TIMEOUT_MIN;
			timer = this.scheduleTimer(electionTimeOut, TIMER_ID);
			
			int term = mConfig.getCurrentTerm();
			System.out.println("S" + mID + "." + term + ": switched to follower mode.");
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
			int vote = term;
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
			return result;
		}
	}

	// @param id of the timer that timed out
	public void handleTimeout(int timerID) {
		// every single class has go- which is called for each mode
		synchronized (mLock) {
			// cancel timer and become candidate
			if(timerID == this.TIMER_ID) {
				timer.cancel();
				// increment the term when you convert from follower to candidate- can be done
				// in the candidate
				CandidateMode candidateMode = new CandidateMode();
				RaftServerImpl.setMode(candidateMode);
			}
		}
	}
}
