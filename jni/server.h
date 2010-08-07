/*
 * server.h
 * --------------------------
 * Contains functions for building the buffer,
 * sending it to the server, and receiving and
 * storing the return data
 */

#define SERVER_IP "71.244.112.67"
#define PORTNUM 2000
#define TPoints 500000

//General variables
float x[TPoints];
float y[TPoints];
int element[TPoints];
int spawn[TPoints];

//Buffer variables
char username[8];
char password[8];
char userlength;
char passlength;
char buffer[3 + 1 + (2 * TPoints * 4) + 200];
int bufferlength;

//Error variable
char* error;

//Socket variables
int sockfd; //The file descriptor for the socket
int n; //Used in sending and recieving data
struct sockaddr_in serv_addr; //The server address struct
struct hostent *server; //Pointer to a hostent struct that is used to set up serv_addr

void adduserpass (void)
{
	/*
	 * Add the username and password to the save buffer. This happens every time, thus the
	 * need for a separate function.
	 */

	int i; //Counter variable

	//Fill in username
	for (i = 4; i < 4 + userlength; i++)
	{
		buffer[i] = username[i];
	}
	//Add the null byte as the delimiter
	buffer[4 + userlength] = (char) 0;
	//Fill in password
	for (i = 4 + userlength + 1; i < 4 + userlength + 1 + passlength; i++)
	{
		buffer[i] = password[i];
	}
	//Add the null byte as the delimiter
	buffer[4 + userlength + 1 + passlength] = 0;
}


void buildbuffer(int type)
{
	int i = 0;
	//Save data
	if(type == 0)
	{
		//The length of the packet, 3 bytes for length, 1 byte for command, rest for data
		bufferlength = 3 + 1 + (userlength+1) + (passlength+1) + (2*TPoints*4);

		//Fill in the 3 length bytes
		buffer[0] = (char)(bufferlength >> 16);
		buffer[1] = (char)(bufferlength % (256 * 256) >> 8);
		buffer[2] = (char)(bufferlength % (256 * 256 * 256));

		//The command byte is 0
		buffer[3] = (char)0;

		//Put in username and password
		adduserpass();

		//Fill in the save data
		for (i = 4 + (userlength+1) + (passlength+1); i < (2*TPoints*4) + (userlength+1) + (passlength+1); i++)
		{
			buffer[i] = (char) (spawn[i / 8] >> 8);
			buffer[++i] = (char) (spawn[(i - 1) / 8] % 256);
			buffer[++i] = (char) ((int)(x[(i - 2) / 8]) >> 8);
			buffer[++i] = (char) ((int)(x[(i - 3) / 8]) % 256);
			buffer[++i] = (char) (((int)y[(i - 4) / 8]) >> 8);
			buffer[++i] = (char) ((int)(y[(i - 5) / 8]) % 256);
			buffer[++i] = (char) (element[(i - 6) / 8] >> 8);
			buffer[++i] = (char) (element[(i - 7) / 8] % 256);
		}

		//SEND DATA HERE
	}
	//Load a save
	else if (type == 1)
	{
		//Length of the packets: 3 bytes for length, 1 byte for command, rest of user and pass data
		bufferlength = 3 + 1 + (userlength + 1) + (passlength + 1);
		//Fill in length data
		buffer[0] = (char)(bufferlength >> 16);
		buffer[1] = (char)(bufferlength % (256 * 256) >> 8);
		buffer[2] = (char)(bufferlength % (256 * 256 * 256));
		//Command byte is 1
		buffer[3] = (char)1;

		//Put in username and password
		adduserpass();

		//Add the data for what file
		//SEND DATA HERE
	}
	//Validate username and password
	else if (type == 2)
	{
		//Length of the packets: 3 bytes for length, 1 byte for command, rest of user and pass data
		bufferlength = 3 + 1 + (userlength + 1) + (passlength + 1);
		//Fill in length data
		buffer[0] = (char)(bufferlength >> 16);
		buffer[1] = (char)(bufferlength % (256 * 256) >> 8);
		buffer[2] = (char)(bufferlength % (256 * 256 * 256));
		//Command byte is 2
		buffer[3] = (char)2;

		//Put in username and password
		adduserpass();

		//SEND DATA HERE
	}
	//Register a username with a password
	else if (type == 3)
	{
			//Length of packet: 3 bytes for length, 1 byte for command, rest for user and pass data
			bufferlength = 3 + 1 + (userlength + 1) + ( passlength + 1 );
			//Fill in length bytes
			buffer[0] = (char)(bufferlength >> 16);
			buffer[1] = (char)(bufferlength % (256 * 256) >> 8);
			buffer[2] = (char)(bufferlength % (256 * 256 * 256));
			//Command is 3
			buffer[3] = (char)3;

			//Put in username and password
			adduserpass();

			//SEND DATA HERE
	}
	//Send custom element data (still not used)
	else if (type = 4)
	{
		/*
		int length = 3 + 1 + (TElements * 2);
		buffer[0] = (char)(length >> 16);
		buffer[1] = (char)(length % (256 * 256) >> 8);
		buffer[2] = (char)(length % (256 * 256 * 256));
		buffer[3] = (char)1;
		for (i = 4; i < 2 * TElements; i++)
		{
			buffer[i] = (char) (colliseelement1[i / 2]);
			buffer[++i] = (char) (collision[22][(i - 1) / 2]);
		}
		*/
	}
}

int sendbuffer(void)
{
	sockfd = socket(AF_INET, SOCK_STREAM, 0); //Create a socket using IPv4 and TCP
	if(sockfd < 0)
	{
		error = "Could not create socket";
		return 0;
	}
	server = gethostbyname(SERVER_IP); //Create the hostent using server IP
	bzero((char*) &serv_addr, sizeof(serv_addr)); //Clear the serv_addr struct
	serv_addr.sin_family = AF_INET; //Use IPv4
	bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length); //Use the hostent to fill the serv_addr struct
	serv_addr.sin_port = htons(PORTNUM); //Set up the port number using network order
	if(connect(sockfd, &serv_addr, sizeof(serv_addr)) < 0)
	{
		error = "Could not connect";
		return 0;
	}

	while(bufferlength > 0)
	{
		n = write(sockfd, buffer, bufferlength);
		if(n < 0)
		{
			error = "Could not write to socket";
			return 0;
		}
		bufferlength -= n;
	}

	return 1;
}
