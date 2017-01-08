package edu.cofc.csci340;

/**
 * 
 * @author CSCI 340 Operating Systems
 *
 */
public class TTimer implements Runnable {
	
	private int id = -1;
	private int count = 0;
	
	/**
	 * 
	 * @param id
	 */
	public TTimer( int id ) {
		
		this.id = id;
		count = 0;
		
	} // end constructor

	/**
	 * 
	 */
	public void run() {
		
		runForEva();
		
	} // end run() method
	
	/**
	 * 
	 */
	public void runForEva() {
		
		while ( true ) {	
			
			try {
				
				System.out.printf("[ID=%d] CNT=%d\n", id, count++ );
				Thread.sleep( 1000*id );
				
			} catch (InterruptedException e) {
				
				count = 0;
				System.out.printf("Interrupt ID=%d ... reseting count\n", id );
				
				
			}
			
		}
		
	} // end runForEva() method

} // end TTimer class definition
