package edu.duke.raft;
import java.util.Timer;

public class CandidateMode extends RaftMode {
	Timer checkVoteTimer;
	Timer electionTimeout;
	
	int timerID;

	
  public void go () {
	  // clear ref responses
	  // reset the election timer
	  // schedule the timer
	  
    synchronized (mLock) {
      int term = 0;      
      System.out.println ("S" + 
			  mID + 
			  "." + 
			  term + 
			  ": switched to candidate mode.");
    }
    
    // increase current term
    int currentTerm = mConfig.getCurrentTerm ();
    currentTerm+=1;
    
    // if election times out then call for votes
    for(int i =1; i <= mConfig.getNumServers(); i++) {
    		remoteRequestVote(i, currentTerm, mID, mLog.getLastIndex(), mLog.getLastTerm());
    }
    
   
    
    
//    mode.remoteRequestVote(mID, currentTerm, candidateID, lastLogIndex, lastLogTerm);
    
    
  }
  
  
  // have a checkVote time out function , array 
  // count then - cancel both timers before you set mode to leader
  

  // @param candidate’s term
  // @param candidate requesting vote
  // @param index of candidate’s last log entry
  // @param term of candidate’s last log entry
  // @return 0, if server votes for candidate; otherwise, server's
  // current term 
  public int requestVote (int candidateTerm,
			  int candidateID,
			  int lastLogIndex,
			  int lastLogTerm) {
    synchronized (mLock) {
      int term = mConfig.getCurrentTerm ();
      int result = term;
      return result;
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
  public int appendEntries (int leaderTerm,
			    int leaderID,
			    int prevLogIndex,
			    int prevLogTerm,
			    Entry[] entries,
			    int leaderCommit) {
    synchronized (mLock) {
      int term = mConfig.getCurrentTerm ();
      int result = term;
      return result;
    }
  }

  // @param id of the timer that timed out
  public void handleTimeout (int timerID) {
    synchronized (mLock) {
    }
  }
}
