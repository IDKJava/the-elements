/*
 * elementproperties.h
 * ----------------------------------------
 * Contains all the arrays that define the element properties.
 * When adding a new element, you need to add it to all of
 * these arrays as well.
 *
 * Properties list:
 * 		-red
 * 		-green
 * 		-blue
 *
 *
 */

#define TElements 25

//Define the color of the element
unsigned char red[] =   {255, 0,   157,   0,   0, 255, 200, 255, 255, 143, 204, 100, 255, 255, 135, 176,   0, 150, 220, 255,  50, 200,   0,  49, 255};
unsigned char green[] = {213, 0,   157,   0, 255,   0, 200, 255, 255,  80,  51, 100, 255, 211,  25, 176,   0, 255, 220, 255, 100, 255, 213,  35, 213};
unsigned char blue[] =  {85,  255, 157,   0,   0,   0, 255, 255, 255,  80,   0, 100, 143, 115,  30, 164,   0,  50, 255, 230, 200, 255,  85,   7,  85};

//Define each element's fall velocity
int	fallvel[] =	{1, 1, 0, 0, 0, -1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, -1, 1, -1, 1, 1, 0, 1, 1, 1};

//collision data (LOAD FROM A FILE MAYBE?)
//-1 -eraser stuff, shouldn't happen
// 0 -solid solid
// 1 -solid liquid
// 2 -liquid solid -- DNE ANYMORE
// 3 -liquid liquid
// 4 -water plant
// 5 -nonflammable fire
// 6 -flammable fire
// 7 -fire fire
// 8 -ice water
// 9 -fire ice
//10 -hot ice
//11 -oil hot
//12 -magma water
//13 -stone hot
//14 -stone stone
//15 -hot C4
//16 -hot C4++
//17 -magma destructable wall
//18 -acid meltable
//19 -acid water
//20 -steam ice/water
//21 -steam steam
//22 -steam other
//23 -water fire
//24 -salt water
//25 -salt ice
//26 -salt-water plant
//27 -sand-hot

//Array defining what element custom element 1 acts as in a collision with each element
int colliseelement1[TElements] =
{
		0, //Sand 0
		0, //Water 1
		0, //Wall 2
		0, //Eraser 3
		0, //Plant 4
		0, //Fire 5
		0, //Ice 6
		0, //Generator 7
		0, //Spawn 8
		0, //Oil 9
		0, //Magma 10
		0, //Stone 11
		0, //C4 12
		0, //C++ 13
		0, //Fuse 14
		0, //Destructable Wall 15
		0, //Wind 16
		0, //Acid 17
		0, //Steam 18
		0, //Salt 19
		0, //Salt-water 20
		0, //Glass 21
		0, //Custom 1 22
		0, //Custom 2 23
		0, //Custom 3 24
};

//Defines the density of each element
int density[] =
{
		16, //Sand:
		10, //Water:
		30,	//Wall:
		-1,	//Eraser:
		15,	//Plant:
		-1,	//Fire:
		9,	//Ice:
		30,	//Generator:
		30,	//Spawn:
		5,	//Oil:
		15,	//Magma:
		30,	//Stone:
		30,	//C4:
		30,	//C4++:
		15,	//Fuse:
		30,	//Wall, Destructible:
		-1,	//Wind:
		8,	//Acid:
		1,	//Steam:
		30,	//Salt:
		11,	//Salt-Water:
		30,	//Glass:
		1,	//Custom 1:
		16,	//Mud:
		1,	//Custom 3:
};
//Is it solid (i.e. fallvel == 0)
//As far as I can tell, it's not used yet
int solid[] =
{
		0,	//Sand:
		0,	//Water:
		1,	//Wall:
		0,	//Eraser:
		1,	//Plant:
		0,	//Fire:
		0,	//Ice:
		0,	//Generator:
		0,	//Spawn:
		0,	//Oil:
		0,	//Magma:
		0,	//Stone:
		0,	//C4:
		0,	//C4++:
		1,	//Fuse:
		1,	//Wall, Destructible:
		0,	//Wind:
		0,	//Acid:
		0,	//Steam:
		0,	//Salt:
		0,	//Salt-Water:
		0,	//Glass:
		0,	//Custom 1:
		0,	//Custom 2:
		0,	//Custom 3:
};
//Does it grow
//As far as I can tell, not used yet
int growing[] =
{
		0, //Sand:
		0, //Water:
		0, //Wall:
		0, //Eraser:
		0, //Plant: (0, because it has it's own special stuff)
		0, //Fire:
		1, //Ice:
		0, //Generator:
		0, //Spawn:
		0, //Oil:
		0, //Magma:
		0, //Stone:
		0, //C4:
		0, //C4++:
		0, //Fuse:
		0, //Wall, Destructible:
		0, //Wind:
		0, //Acid:
		0, //Steam:
		0, //Salt:
		0, //Salt-Water:
		0, //Glass:
		0, //Custom 1:
		0, //Custom 2:
		0, //Custom 3:
};

//Does it condense
//As far as I can tell, not implemented yet
int condensing[] =
{
		-1, //Sand:
		-1,	//Water:
		-1,	//Wall:
		-1,	//Eraser:
		-1,	//Plant:
		-1,	//Fire:
		-1,	//Ice:
		-1,	//Generator:
		-1,	//Spawn:
		-1,	//Oil:
		-1,	//Magma:
		-1,	//Stone:
		-1,	//C4:
		-1,	//C4++:
		-1,	//Fuse:
		-1,	//Wall, Destructible:
		-1,	//Wind:
		-1,	//Acid:
		1,	//Steam:
		-1,	//Salt:
		-1,	//Salt-Water:
		-1,	//Glass:
		-1,	//Custom 1
		-1,	//Custom 2
		-1,	//Custom 3
};

//Does it burn things?
//As far as I can tell, not implemented yet
int fireburn[] =
{
		0, //Sand:
		0, //Water:
		0, //Wall:
		0, //Eraser:
		0, //Plant:
		1, //Fire:
		0, //Ice:
		0, //Generator:
		0, //Spawn:
		0, //Oil:
		0, //Magma:
		0, //Stone:
		0, //C4:
		0, //C4++:
		0, //Fuse:
		0, //Wall, Destructible:
		0, //Wind:
		0, //Acid:
		0, //Steam:
		0, //Salt:
		0, //Salt-Water:
		0, //Glass:
		0, //Custom 1
		0, //Custom 2
		0, //Custom 3
};

//How explosive is it?
//As far as I can tell, not implemented yet
int exploness[] =
{
		0, //Sand:
		0, //Water:
		0, //Wall:
		0, //Eraser:
		0, //Plant:
		0, //Fire:
		0, //Ice:
		0, //Generator:
		0, //Spawn:
		0, //Oil:
		0, //Magma:
		0, //Stone:
		10,//C4:
		20,//C4++:
		0, //Fuse:
		0, //Wall, Destructible:
		0, //Wind:
		0, //Acid:
		0, //Steam:
		0, //Salt:
		0, //Salt-Water:
		0, //Glass:
		0, //Custom 1
		0, //Custom 2
		0, //Custom 3
};
