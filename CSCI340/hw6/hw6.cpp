#include <stdio.h>
#include <stdlib.h>
#include <pcap.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "sniffex.h"

int main(int argc, char **argv) {
  
  char* inet;
  char* host;
  char* netmask;
  
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_if_t *alldevs, *d;
  
  bpf_u_int32 netp; /* ip */
  bpf_u_int32 maskp;/* subnet mask */
  
  struct in_addr addr;
  std::vector<std::string> ndevs;
  std::vector<char*> inets;
  
  /**
   * 
   * Ask the user to select a network device on the
   * host machine.
   * 
   */
  
  if ( pcap_findalldevs( &alldevs, errbuf ) == -1 ) {
    
    std::cerr<<"Error in pcap_findalldevs: "<< errbuf<< std::endl;
    exit(1);
    
  }
  
  int i = 0;
  int selected_dev = 0;
  
  std::cout<<"------------------------------------------"<< std::endl;
  std::cout<<"List of available network devices:"<<std::endl;
  std::cout<<"------------------------------------------"<< std::endl;
  
  for ( d=alldevs; d; d=d->next ) {
    
    ndevs.push_back( d->name );
    
    
    inet = (char*)malloc(16);
    inet[0] = '\0';
   
    for ( pcap_addr_t *a=d->addresses; a!=NULL; a=a->next ) {
      if (a->addr->sa_family == AF_INET )   
	strncat(inet, inet_ntoa( ( ( struct sockaddr_in* )a->addr )->sin_addr ), 16 );
    }
    
    inets.push_back( inet );
    
    std::cout<<"("<<(i)<<") "<<ndevs[i++]<<" ";
    
    if ( d->description )
      std::cout<<"\t\""<<d->description<<"\""<<std::endl;
    else
      std::cout<<""<< std::endl;
      
  }
  
	// SPEC B, prompt user to select network device

  std::cout<<"------------------------------------------"<< std::endl;
  std::cout<<"Please select a device [1 through "<<i<<"]: ";
  std::cin>>selected_dev;
  std::cout<<"Selected device: "<<ndevs[selected_dev-1]<<std::endl;
  

  /*
	TODO: 1

	Use pcap_lookupnet function to find network number
	and mask for the selected device.

	http://linux.die.net/man/3/pcap_lookupnet

	TODO: 2
		
	Display host, inet, and mask information
	for the selected device.

	Hints:
	 1. use inet_ntoa to find host info.
	 2. use netp and maskp (found above) to display inet
	    and netmask info.

	http://linux.die.net/man/3/inet_ntoa

  */
	
	// Convert that C++ vector stuff to a char
	const char* dev; // selected device
	dev = (char*) malloc(16);
	dev = ndevs[selected_dev-1].c_str(); // Thank you Stack Overflow

	// Find network number and mask for dev
	if (pcap_lookupnet(dev, &netp, &maskp, errbuf) == -1) {
		fprintf(stderr, "pcap_lookupnet(): %s\n", errbuf);
		exit(1);
	}

	// SPEC C, display IPv4 info for selected network device
	// Print host

	addr.s_addr = netp;
	host = inet_ntoa(addr);

	if (host == NULL) {
		printf("inet_ntoa(): %s\n", errbuf);
		exit (1);
	}

	printf("host %s\n", host);

	// Print IP address?
	inet = inet_ntoa(addr);
	printf("inet %s\n", inet);

	// Print netmask

	addr.s_addr = maskp;
	netmask = inet_ntoa(addr);

	if (netmask == NULL) {
		printf("inet_ntoa(): %s\n", errbuf);
		exit(1);
	}

	printf("netmask %s\n", netmask);
	printf("------------------------------------------\n");

	// END TODO 1 and 2  

  
  for ( int i=0; i<inets.size(); i++ ) free( inets[i] ); 
  
  /**
   * 
   * Live packet capture
   * 
   * 
   */
   
  std::cout<< "Starting live packet capture!"<<std::endl;
 
  const u_char* packet;
  struct pcap_pkthdr hdr;
  
	struct sniff_ethernet* eth_hdr;
	struct sniff_ip *ip_hdr; // added IP header
	struct sniff_tcp *tcp_hdr; // added TCP header

	u_int size_ip;
	u_int size_tcp;

	struct bpf_program fp; // filters for SPEC G
	char filter_exp[] = "port 80";

	char* sip;
	char* dip;
  int sport;
  int dport;
  pcap_t* cap_desc;

  struct timespec nano_time_spec;
  nano_time_spec.tv_nsec=100000;
  nano_time_spec.tv_sec=0;

	int timeout = nano_time_spec.tv_nsec; // int the nanoseconds?

  /*
	TODO: 3

	Start live packet capture using pcap_open_live function.

	http://linux.die.net/man/3/pcap_open_live

  */
  
	// SPEC D, start a live packet capture
	cap_desc = pcap_open_live(dev, BUFSIZ, 1, timeout, errbuf);
	
	if (cap_desc == NULL) {
		printf("pcap_open_live(): %s\n", errbuf);
		exit(1);
	}


	// END TODO 3
  
  while ( true ) {
  
    packet = pcap_next( cap_desc, &hdr ); 
  
    if ( packet != NULL ) {

      /*
	TODO: 4

	For each packet inspect the headers as follows:
		1. Get the ETHERNET frame
    2. Get the IP datagram
		3. Get the TCP segment
	  4. If the ether_type == ETHERTYPE_IP AND the ip_p == IPPROTO_TCP then
		   display the sender and receiver info if either has port = 80.

      */
      
			// SPEC G, Port 80 filters...
			if(pcap_compile(cap_desc, &fp, filter_exp, 0, netp) == -1) {
				printf("pcap_compile(): %s\n", errbuf);
				exit(2);
			}
			
			if(pcap_setfilter(cap_desc, &fp) == -1) {
				printf("pcap_setfilter(): %s\n", errbuf);
				exit(2);
			}

			// SPEC E, only capture IP datagrams and TCP segments
      eth_hdr = (struct sniff_ethernet *) packet;

			if(ntohs(eth_hdr->ether_type) == ETHERTYPE_IP) {

				ip_hdr = (struct sniff_ip *)(packet + SIZE_ETHERNET);
				size_ip = IP_HL(ip_hdr) * 4;

				sip = inet_ntoa(ip_hdr->ip_src); // SPEC F
				dip = inet_ntoa(ip_hdr->ip_dst);

				if(ip_hdr->ip_p == IPPROTO_TCP) {
					tcp_hdr = (struct sniff_tcp*)(packet + SIZE_ETHERNET + size_ip);
					size_tcp = TH_OFF(tcp_hdr) * 4;

					sport = ntohs(tcp_hdr->th_sport);
					dport = ntohs(tcp_hdr->th_dport);

				}
			}
			
			// SPEC H, Output format is correct
			std::cout<<"Source ["<<sip<<" : "<<sport<<"] -> Destination ["<<dip<<" : "<<dport<<"]"<<std::endl;

			// END TODO 4

    }
  
  } 
  
  return 0;

} 
