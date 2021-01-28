#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <algorithm>
#include <initializer_list>
#include <time.h>

using namespace std;

#define speed 4

#define sleep usleep(500000 / speed);

#define repeat(x) for (int i = x; i--;)

#define columns 3

//Clear the screen - Unix call
#define clearScreen cout << "\033c";

//Global variables
int moves = 0;
time_t start;
string inClaw;
int maxBlocksOnLocation;
int currentLocation;

//Input file name
string inputFile = "States.txt";

//Vector of strings for the beginning and end states, holds 2x columns
vector<string> loc;

//Vector holding all 2x columns loc vectors
vector<vector<string> > table;

//Vector holding the size of locs at before sorting
vector<int> tableSizes;

//Pickup a block with the claw from location
void pickup(int location)
{
  inClaw = table[location].back();
  table[location].pop_back();
}

//Putdown a block from the claw to location
void putdown(int location)
{
  table[location].push_back(inClaw);
}

//Pickup and putdown a block from provided locations
void stack(int beginning, int end)
{
  pickup(beginning);
  putdown(end);
}

//Pickup and putdown a block from provided locations
void unstack(int beginning, int end)
{
  pickup(beginning);
  putdown(end);
}

//Print the header with the title, name and moves and time elapsed
void printHeader(int location)
{
  clearScreen;

  //Header
  cout << string(50, '*') << endl;
  cout << "*" << string(17, ' ') << "AI Block Sorting" << string(15, ' ') << "*\n";
  cout << "*" << string(48, ' ') << "*\n";
  cout << "* David Bebawy ©" << string(33, ' ') << "*\n";

  //Starting header
  if (location == -2)
  {
    cout << "*" << string(48, ' ') << "*\n";
    cout << string(50, '*') << endl;
    return;
  }

  cout << "* Moves: " << moves << "\t" << string(6, ' ') << "Time Elapsed: ";

  //0 seconds while showing the provided unsorted state
  if (location == -1)
    cout << "0 seconds" << string(4, ' ') << "*\n";
  else //Show time elapsed
    cout << difftime(time(0), start) << " seconds\t *\n";

  cout << string(50, '*') << endl;
}

//Print the claw, either empty or holding one block
void printClaw(int location, string arm)
{
  int margin = location * 10;

  repeat(3) cout << string(margin + 3, ' ') << string(3, '*') << "\n";
  cout << string(margin, ' ') << string(9, '*') << "\n";

  if (arm != "") //The arm holds a block
  {
    cout << string(margin, ' ') << "* " << string(5, '-') << " *\n";
    cout << string(margin, ' ') << "* | " << arm << " | *\n";
    cout << string(margin, ' ') << "* " << string(5, '-') << " *\n";
  }
  else //The arm is empty
    repeat(3) cout << string(margin, ' ') << "*  " << string(3, ' ') << "  *\n";
}

//Print the blocks on the provided columns
void printBlocks(int location)
{
  int margin = location * 15;

  // //Get the max height of blocks
  // int maxheight = table[0].size();
  // for (int i = 1; i < columns * 2; i++)
  //   if (table[i].size() > maxheight)
  //     maxheight = table[i].size();

  // //Print newlines based on maxheight
  // for (int i = 7; i > blocks; i--)
  //   cout << string(3, '\n');

  //Print a block or empty space
  for (int i = maxBlocksOnLocation; i > 0; i--)
  {
    for (int j = 0; j < columns; j++)
    {
      if (table[j].size() >= i)
        cout << string(2, ' ') << string(5, '-') << string(3, ' ');
      else
        cout << string(10, ' ');
    }
    cout << endl;

    for (int j = 0; j < columns; j++)
    {
      if (table[j].size() >= i)
        cout << string(2, ' ') << "| " << table[j].at(i - 1) << " |" << string(3, ' ');
      else
        cout << string(10, ' ');
    }
    cout << endl;

    for (int j = 0; j < columns; j++)
    {
      if (table[j].size() >= i)
        cout << string(2, ' ') << string(5, '-') << string(3, ' ');
      else
        cout << string(10, ' ');
    }
    cout << endl;
  }
  cout << string(10 * columns - 1, '*') << endl;
}

//Print helper, calls three print functions: Header, Claw & Blocks, and sleeps based on the define speed
void printHelper(int location = 0, string arm = "")
{
  printHeader(location);

  //Print start position, no claw
  if (location == -1)
    cout << string(7, '\n');
  else
    printClaw(location, arm);

  printBlocks(location);

  sleep;
}

//Move function calls pickup and putdown, and increases move counter
// calls printHelper 4 times: premove, block in claw, claw above new location, postmove

void move(int x, int y, bool print)
{
  if (print) printHelper(x);
  pickup(x);
  if (print) {
  printHelper(x, inClaw);
  printHelper(y, inClaw);
  }
  putdown(y);
  moves++;
  if (print) printHelper(y);
  
  //Record the max blocks put on one location
  if (!print) {
    for (int i = 0; i < columns; i++) {
      if (maxBlocksOnLocation < table[i].size()) maxBlocksOnLocation = table[i].size();
    }
  }
}

//check if the item matching blocks are already in the right order
void finalStateBlockCheck(int location = currentLocation)
{
  for (int i = tableSizes[location]; i < tableSizes[location + columns] && i < table[location].size(); i++)
  {
    if (table[location].at(i) == table[location + columns].at(i))
      tableSizes[location]++;
    else
      break;
  }
}

//Determine the overflow location with next needed block, return it and move excess block to the other location
int determineOverflow(bool print)
{
  //Use x as placeholder for the offset.
  //The 3 columns are now currentLocation, ((currentLocation + x) % columns) and ((currentLocation + x * 2) % columns)
  int x;

  //Move the excess blocks to the other location that does not contain the first needed block
  for (int i = table[currentLocation].size(); i > tableSizes[currentLocation]; i--)
  {
    x = 2;
    int secondLocation = (currentLocation + x * 2) % columns;
    for (int i = 0; i < table[secondLocation].size(); i++)
    {
      if (table[secondLocation].at(i) == table[currentLocation + columns].at(tableSizes[currentLocation]))
      {
        x = 1;
        break;
      }
    }
    move(currentLocation, secondLocation, print);
  }

  return x;
}

//Sorting helper function moves the blocks from firstLocation to currentLocation to mach the end state or to the secondLocation
void sortingHelper(int x, bool print)
{
  bool changeColumn = false;
  int firstLocation = (currentLocation + x) % columns;
  int secondLocation = (currentLocation + x * 2) % columns;

  //While currentLocation is less than end state currentLocationand x is less than end state x
  while (tableSizes[currentLocation] < tableSizes[currentLocation + columns] && tableSizes[firstLocation] < table[firstLocation].size())
  {
    //Iterate over the blocks in x, check for the next needed block in currentLocation
    for (int i = 0; i < table[firstLocation].size(); i++)
    {
      if (table[firstLocation].at(i) == table[(currentLocation + columns)].at(tableSizes[currentLocation]))
        changeColumn = true;
    } //BETTER WAY, CHECK 1 ITEM IF IN VECTOR

    if (changeColumn)
    {
      //If the top block is the next needed block in currentLocation, move it to currentLocation, increase the table size of currentLocation, and reset changeColumn
      if (table[firstLocation].back() == table[currentLocation + columns].at(tableSizes[currentLocation]))
      {
        move(firstLocation, currentLocation, print);
        tableSizes[currentLocation]++;
        changeColumn = false;
      }
      // else if ((table[firstLocation].size() == table[secondLocation].size() + 1) && tableSizes[secondLocation] < tableSizes[secondLocation + columns] )
      else if (table[secondLocation].size() <= tableSizes[secondLocation] && tableSizes[secondLocation] < tableSizes[secondLocation + columns])
      {
        if (table[firstLocation].back() == table[secondLocation + columns].at(tableSizes[secondLocation]))
          tableSizes[secondLocation]++;
        move(firstLocation, secondLocation, print);
      }
      //Else, move the block to x + 2 (the third location)
      else
      {
        move(firstLocation, secondLocation, print);
        finalStateBlockCheck(secondLocation);
      }
    }
    //Block is not found, return.
    else
      return;
  }
}

//Sorting function iterates over each location, checking if the beginning state matches the final state
//Move all excess blocks to the other locations and then move the blocks back to the correct location
void sorting(int locations[], bool print)
{
  for (int t = 0; t < columns; t++)
  {
    currentLocation = locations[t];
    finalStateBlockCheck();

    if (tableSizes[currentLocation] != tableSizes[currentLocation + columns])
    {
      int x = determineOverflow(print);

      //Move blocks from the other locations till all blocks for the current location are at the correct final spot
      while (tableSizes[currentLocation] < tableSizes[currentLocation + columns])
      {
        sortingHelper(x, print);
        sortingHelper(x * 2, print);
      }
    }
  }
}

//Test all permutation of the location, sort based on the least moves
void determineLocationPermutation()
{
  //Initialize locations
  int locations[columns];
  for (int i = 0; i < columns; i++)
    locations[i] = i;
  
  int leastMoves = 0;
  int maxBlocks;
  int locations2[columns];
  vector<vector<string> > originalTable = table;
  vector<int> originalTableSizes = tableSizes;
  do
  {
    moves = 0;
    maxBlocksOnLocation = 0;
    table = originalTable;
    tableSizes = originalTableSizes;
    sorting(locations, false);
    if (!leastMoves || moves < leastMoves)
    {
      leastMoves = moves;
      maxBlocks = maxBlocksOnLocation;
      copy(begin(locations), end(locations), begin(locations2));
    }
  } while (next_permutation(locations, locations + columns));
  moves = 0;
  maxBlocksOnLocation = maxBlocks;
  table = originalTable;
  tableSizes = originalTableSizes;

  printHelper(-1);

  sorting(locations2, true);
}

//Read input file and save the states in table vector
void readFile()
{
  string myText;

  //Read from input text file
  ifstream MyReadFile(inputFile);

  //Print error if file cannot be opened
  if (!MyReadFile)
  {
    printHeader(-2);
    cout << endl;
    cerr << "Unable to open file " + inputFile + "\n";
    exit(1); // call system to stop
  }

  //Read the beginning and end states from file
  for (int i = 0; i < table.size(); i++)
  {
    getline(MyReadFile, myText);
    stringstream blocks(myText);
    string token;
    while (blocks >> token)
      table[i].push_back(token);
  }
}

int main()
{
  //6 vector<string> 3 for the pre, 3 for post
  repeat(columns * 2) table.push_back(loc);

  readFile();

  //Set the first tableSizes entries sizes to 0
  repeat(columns) tableSizes.push_back(0);

  //Set the last tableSizes entries sizes to the end state sizes.
  for (int i = columns; i < columns * 2; i++)
    tableSizes.push_back(table[i].size());


  start = time(0);

  determineLocationPermutation();

  cout << "\nThe blocks have been sorted successfully in " << moves << " moves.\n\n";
}