
// -----------------------------------
// CSIS 614 - Advanced Operating Systems
// Summer 2016
// server.h header file
// Project 1
//
// -----------------------------------

#include <string.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include "server.h"

// ------------------------------------
// Function prototype that creates a socket and 
// then binds it to the specified port_number 
// for incoming client connections
// 
//
// Arguments:	port_number = port number the server 
//				socket will be bound to.
//
// Return:      Socket file descriptor (or -1 on failure)
//

int bind_port( unsigned int port_number ) {

	// -------------------------------------
	// NOTHING TODO HERE :)
	// -------------------------------------
	// Please do not modify

	int socket_fd;
	int set_option = 1;

    struct sockaddr_in server_address;
     
    socket_fd = socket( AF_INET, SOCK_STREAM, 0 );

    setsockopt( socket_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&set_option, sizeof( set_option ) );

    if (socket_fd < 0) return FAIL;

    bzero( (char *) &server_address, sizeof(server_address) );

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons( port_number );

    if ( bind( socket_fd, (struct sockaddr *) &server_address, sizeof(server_address) ) == 0 ) {

    	return socket_fd;

    } else {

    	return FAIL;

    }

} // end bind_port function

// ------------------------------------
// Function prototype that accepts a client
// socket connection
// 
//
// Arguments:	server file descriptor
//
// Return:      Termination status of client
//				( 0 = No Errors, -1 = Error )
//
int accept_client( int server_socket_fd ) {

	int exit_status = OK;
	int client_socket_fd = -1;
	int url_idx = 0;
	int i = 0;

	socklen_t client_length; 
	pid_t pid;
	struct sockaddr_in client_address;

	char request[512];
	char response[512];
	char kv_pairs[100];
	char row[200];
	char table_rows[200];

	char* token;
	char* p;
	char* pp;
	char* entity_body;
	char tmp_c;
	char* end_html = "</table></body></html>";
	char* table_rows_error = "<tr><td colspan=2>URL not properly formatted!</td></tr>";

	client_length = sizeof( client_address );

    client_socket_fd = accept( server_socket_fd, (struct sockaddr *) &client_address, &client_length );

    // -------------------------------------
	// TODO:
	// -------------------------------------
	// You complete child signal handler code to remove child process from process 
	// table (i.e. reap the child)
	// -------------------------------------

	pid = fork();

    if ( client_socket_fd >= 0 && pid == 0 ) {

    	close( server_socket_fd );

	    bzero( table_rows, 200 );
	    bzero( request, 512 );

	    read( client_socket_fd, request, 511 );

	    p = request;
	    
	    if ( DEBUG ) printf("Here is the http message:\n%s\n\n", request );

	    // -------------------------------------
		// TODO:
		// -------------------------------------
		// Generate the correct http response when a GET or POST method is sent
		// from the client to the server.
		// 
		// In general, you will parse the request character array to:
		// 1) Determine if a GET or POST method was used
		// 2) Then retrieve the key/value pairs (see below)
		// -------------------------------------

		/*
		------------------------------------------------------
		GET method key/values are located in the URL of the request message
		? - indicates the beginning of the key/value pairs in the URL
		& - is used to separate multiple key/value pairs 
		= - is used to separate the key and value

		Example:

		http://localhost/?first=brent&last=munsell

		two &'s indicated two key/value pairs (first=brent and last=munsell)
		key = first, value = brent
		key = last, value = munsell
		------------------------------------------------------
		*/

		/*
		------------------------------------------------------
		POST method key/value pairs are located in the entity body of the request message
		? - indicates the beginning of the key/value pairs
		& - is used to delimit multiple key/value pairs 
		= - is used to delimit key and value

		Example:

		first=brent&last=munsell

		two &'s indicated two key/value pairs (first=brent and last=munsell)
		key = first, value = brent
		key = last, value = munsell
		------------------------------------------------------
		*/

		if ( request[0] == 'G' && request[1] == 'E' && request[2] == 'T' ) {

			entity_body = "<html><body><h2>GET Operation</h2><table border=1 width=\"50%\"><tr><th>Key</th><th>Value</th></tr>";

			url_idx = 4;
			bzero( kv_pairs, 100 );
			i = 0;

			while ( p[url_idx] != ' ' )  { 
				
				if ( p[url_idx] != '/' && p[url_idx] != '?' ) {

					kv_pairs[i++] = p[url_idx];

				}

				url_idx++;

			}

			kv_pairs[i] ='\0';

		} else {

			entity_body = "<html><body><h2>POST Operation</h2><table border=1 width=\"50%\"><tr><th>Key</th><th>Value</th></tr>";

			url_idx = strlen( p ) - 1;
			i = 0;

			while ( p[url_idx] != '\n' ) {
			
				kv_pairs[i++] = p[url_idx--];

			}

			kv_pairs[i] = '\0';

			for ( i = 0; i<(int)(strlen( kv_pairs )/2); i++ ) {

				tmp_c = kv_pairs[ i ];
				kv_pairs[i] = kv_pairs[strlen( kv_pairs ) - 1 - i ];
				kv_pairs[strlen( kv_pairs ) - 1 - i ] = tmp_c;
				
			}

		} 

		if ( strlen( kv_pairs ) > 0 ) {

			if ( DEBUG ) printf("URL Key/Value Pairs = %s\n", kv_pairs );

			token = strtok( kv_pairs, "&" );

			p = pp = token;
			i = 0;

			while ( p[i++] != '=' ) pp++;
			pp++; p[i-1] = '\0';

			sprintf( row, "<tr><td>%s</td><td>%s</td></tr>", p, pp );
			strcat( table_rows, row );

			while (token != NULL ) {

				token = strtok( NULL, "&" );

				if ( token != NULL ) {

					p = pp = token;
					i = 0;

					while ( p[i++] != '=' ) pp++;
					pp++; p[i-1] = '\0';

					sprintf( row, "<tr><td>%s</td><td>%s</td></tr>", p, pp );
					strcat( table_rows, row );

				}

			}
		
			sprintf( response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s%s%s", ((int)strlen( entity_body ) + (int)strlen( table_rows) + (int)strlen( end_html)), entity_body, table_rows, end_html );

		} else {

			sprintf( response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s%s%s", ((int)strlen( entity_body ) + (int)strlen( table_rows_error) + (int)strlen( end_html)), entity_body, table_rows_error, end_html );

		}

		if ( DEBUG ) printf( "%s\n", response );

	    write( client_socket_fd, response, strlen( response ) );

	    close( client_socket_fd );

	    exit(0);

	} else {

		exit_status = FAIL;

	}

	close( client_socket_fd );

	return exit_status;

} // end accept_client function
