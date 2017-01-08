package edu.cofc.csci340;

import java.util.InputMismatchException;
import java.util.Scanner;

/**
 * 
 * @author CSCI 340 Operating Systems
 *
 */
public class InterruptApp {
	
	/**
	 * 
	 * @param args
	 */
	public static void main( String[] args ) {
		
		Scanner input = new Scanner( System.in );
		
		TTimer ttimer1 = new TTimer( 1 );
		TTimer ttimer2 = new TTimer( 2 );
		
		Thread thread1 = new Thread( ttimer1 );
		Thread thread2 = new Thread( ttimer2 );
		
		thread1.start();
		thread2.start();
		
		int signal = 0;
		
		while (true) {
			 
			System.out.println( "Enter 1 or 2 to Interrupt: ");
			
			try {
			
				signal = input.nextInt();
			
			} catch( InputMismatchException error ) {
				
				signal = 0;
				input.nextLine();
			}
			
			if ( signal == 1 ) {
				
				thread1.interrupt();
				
			} else if ( signal == 2 ) {
				
				thread2.interrupt();
				
			}
			
		}
		
	
	} // end main() method

} // end InterruptApp class definition
