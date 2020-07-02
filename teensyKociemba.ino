/**********************************************************************
* Teensy 4.1 port of Kociemba's algorithm for solving a Rubik's cube.
*
* The (beautiful) original algorithm was designed by H Kociemba:
* c.f. http://kociemba.org/ for details.
*
* This code is a straighforward hack from the c-code available at:
* https://github.com/muodov/kociemba
*
*
* Requirement: the algo. needs 4.3MB of Flash memory for storing the 
* precomputed tables but just a few KB of RAM. It can be speed up by 
* allocating more RAM to store some cache tables in fast memory. 
*
* Usage:
* - allocate more RAM with kociemba::set_memory()
* - solve a cube with kociemba::solve()
*********************************************************************/

#include "Arduino.h"

#include "kociemba/kociemba.h"


// 5 randomly scrambled cube. 
char test_cube[5][55] = {
	"UBRDUFUDLBUBFRDFRDFLURFRBFDDLRLDDFBLRBLULBLURUUFFBRBLD",
	"RDRBUFFUUFRFDRULRFDRLBFBBFBDUDFDDBBRURLDLULLRDLBLBFULU",
	"DRLUUBFBRBLURRLRUBLRDDFDLFUFUFFDBRDUBRUFLLFDDBFLUBLRBD",
	"DLFRUUURLUBDLRBLURBBFLFDFFUURBFDRBBDLDRDLFDURRUFDBFBLL",
	"BUFUUDFBLURRFRLRFBDRFUFRBBFRDDFDDBULLLLDLBRLUURDBBLUFD"
	};


extern "C" uint32_t set_arm_clock(uint32_t frequency);

elapsedMillis em;

char* buf479;				// 479K in DMAMEM
char  buf248[248 * 1024];	// 248K on the stack 


void setup() 
	{
	while (!Serial);
	em = 0;
	buf479 = new char[479 * 1024];	// allocate memory in DMAMEM. 
	kociemba::set_memory(buf479, buf248);	// removing this line slows the computation by a factor of 4 (but saves a lot of RAM...)
	Serial.printf("RAM buffer created in %d ms.\n", (int)em);
	}


void loop() 
	{
	for (auto s : test_cube)
		{
		em = 0;
		set_arm_clock(816000000); // let's boost !
		const char* res = kociemba::solve(s);
		set_arm_clock(600000000); // calm down...
		if (res == nullptr)
			Serial.println("no solution found :(");
		else
			Serial.printf("Solution [%s] found in %d ms.\n", res, (int)em);
		}
	}

/* end of file */





