/*
   Program     : minHeap.cpp
   Description : implements a minHeap with menu
   Programmer  : Sparsh Jain
   Roll  No    : 111601026
   Date        : January 23, 2018
*/

//HW 3.9, 3.16,

#include <iostream>
using namespace std;

//Function to print the menu
void printMenu();

/*
   This implementation uses an array to store the heap of size n from index 1 to n instead of the usual array implementation of 1 to n-1
   class minHeap to implement minHeap with necessary functions
   The maximum size of the heap can be 9999
*/
class minHeap
{
 private:
   int list[10000]; //Stores the heap as array
   int size;        //Stores the size of the heap

 public:
   //Constructor to initialize the size to be 0
   minHeap()
   {
      size = 0;
   }

   //Returns the size of the heap
   int getSize()
   {
      return size;
   }

   //Returns the value at an index of the heap
   int getValue(int index)
   {
      if (index > 0 && index <= size)
         return list[index];
      else
      {
         cout << "Does not exist!!!";
         return -1;
      }
   }

   //Returns the index of the left child if exists, returns -1 otherwise
   int left(int index)
   {
      if (2 * index <= size)
         return (2 * index);
      else
         return -1;
   }

   //Returns the index of the rigth child if exists, returns -1 otherwise
   int right(int index)
   {
      if (2 * index + 1 <= size)
         return (2 * index + 1);
      else
         return -1;
   }

   //Returns the index of the parent of index, -1 if the given index is out of bounds
   int parent(int index)
   {
      if (index < 1 || index > size)
         return -1;
      else
         return index / 2;
   }

   //Returns the minimum element from the heap
   int getMin()
   {
      if (size == 0)
      {
         cout << "Heap Empty!" << endl;
         return -1;
      }
      return list[1];
   }

   //Destroys the heap
   void destroy()
   {
      size = 0;
      cout << "Destroyed entire heap!" << endl;
   }

   minHeap(int array[], int n);             //Overloaded constructor to build a heap from an array of definite size
   void minHeapify(int index);              //Function minHeapify to correct any distortion between the given index and its decendents
   void build(int array[], int n);          //Function build to create a heap from a given array of definite size
   void siftUp(int index);                  //Function siftUp to correct any distortion between the given index and its ancestors
   void insert(int data);                   //Function insert to insert a new element in the heap
   int removeMin();                         //Function removeMin to remove the minimum element from the heap
   void heapSort();                         //Function heapSort to sort the given heap
   void showTree(int node, int offset);     //Function to display the heap in a tree like format
   void printHeap();                        //Function to display the heap as an array
   void decreaseKey(int index, int newKey); //Function to decease the key at an index
};

//main function to run the program
int main()
{
   int choice, n, array[10000];
   minHeap heap;

   //doWhile loop to implement the menu
   do
   {
      printMenu();
      cin >> choice;
      switch (choice)
      {
      case 1: //Create New Heap
         cout << "Enter the number of elements (less than 10000): ";
         cin >> n;
         for (int i = 0; i < n; i++)
         {
            cout << "Enter element #" << i + 1 << ": ";
            cin >> array[i];
         }
         heap.build(array, n);
         break;
      case 2: //Insert an element in the heap
         cout << "Enter the new element: ";
         cin >> n;
         heap.insert(n);
         break;
      case 3: //Delete the minimum element
         n = heap.removeMin();
         cout << "Removed " << n << endl;
         break;
      case 4: //display sorted list(heapSort)
         cout << "Sorted List: ";
         heap.heapSort();
         break;
      case 5: //Display in tree like form
         heap.showTree(1, 1);
         break;
      case 6: //Display the minimum element
         n = heap.getMin();
         cout << "Mininum Element: " << n << endl;
         break;
      case 7: //Display the size of heap
         cout << "Heap Size: " << heap.getSize() << endl;
         break;
      case 8: //Delete the entire heap
         heap.destroy();
         break;
      case 9: //Print the heap
         heap.printHeap();
         break;
      case 10: //Decrease Key
         int index;
         cout << "Enter the index of which you want to decrease the key: ";
         cin >> index;
         if (index < 1 || index > heap.getSize())
         {
            cout << "Does not exist!!" << endl;
            break;
         }
         cout << "Current Value at " << index << ": " << heap.getValue(index) << endl;
         cout << "Enter the new value :";
         cin >> n;
         heap.decreaseKey(index, n);
         break;
      case 11: //Exit
         break;
      default: //Invalid Choice
         cout << "Please Enter Appropriate Choice!" << endl;
      }

   } while (choice != 11); //run until the user asks to exit

   cout << endl;
   return 0;
}

//Constructor to build a heap with a given array of a definite size
minHeap::minHeap(int array[], int n)
{
   build(array, n);
}

//minHeapify corrects any distortion between the given index and its decendents
void minHeap::minHeapify(int index)
{
   int small = index, temp;

   //checking for boundary conditions
   if (index < 1 || index > size)
      return;

   //Updating the small index
   //First check if the child exists or not
   if (left(index) != -1 && list[left(index)] < list[small])
      small = left(index);
   if (right(index) != -1 && list[right(index)] < list[small])
      small = right(index);

   //if small index is changed, swap and call minHeapify on the new index
   if (list[small] != list[index])
   {
      temp = list[small];
      list[small] = list[index];
      list[index] = temp;
      minHeapify(small);
   }
}

//build creates the heap of a given size from a given array by calling minHeapify for all nodes with height more than 0 in ascending order of their heights
void minHeap::build(int array[], int n)
{
   int i;
   size = 0;

   //copy the array
   for (i = 0; i < n; i++)
   {
      size++;
      list[size] = array[i];
      //check for overflow
      if (size == 9999)
      {
         cout << "Heap overflow!" << endl;
         break;
      }
   }

   //call minHeapify as required
   for (i = size / 2; i > 0; i--)
      minHeapify(i);
}

//siftUp corrects any distortion between the given index and its ancenstors
void minHeap::siftUp(int index)
{
   //Check for boundary conditions
   if (index <= 1 || index > size)
      return;

   //If a distortion is found, swap and siftUp its parent
   if (list[index] < list[parent(index)])
   {
      int temp = list[parent(index)];
      list[parent(index)] = list[index];
      list[index] = temp;
      siftUp(parent(index));
   }
}

//insert a new element in the heap
void minHeap::insert(int data)
{
   //check for boundary condition
   if (size == 9999)
   {
      cout << "Heap already Full!" << endl;
      return;
   }

   //update the size and enter data in the end
   size++;
   list[size] = data;

   //arrange the heap by sifting Up the data
   siftUp(size);
}

//remove and return the minimum element from the heap
int minHeap::removeMin()
{
   int result;

   //check for boundary conditions
   if (size == 0)
   {
      cout << "Heap Empty!" << endl;
      return -1;
   }

   //swap the minimum element with the last element and decrease the size
   result = list[1];
   list[1] = list[size];
   list[size] = result;
   size--;
   //Call minheapify if the heap is non empty to correct any distortions due to swapping
   if (size > 0)
      minHeapify(1);
   //return the minimum element
   return result;
}

//display the heap in ascending order
void minHeap::heapSort()
{
   int n = size, array[10000];

   //Use removeMin repeatedly to get ascending order
   for (int i = 1; i <= n; i++)
   {
      int num = removeMin();
      cout << num << " ";
      array[i - 1] = num;
   }
   cout << endl;
   //rebuild the heap to retain the data
   build(array, n);
}

//print the heap in a tree like format from a given node
void minHeap::showTree(int node, int offset)
{
   //Check for boundary conditions
   if (size == 0)
   {
      cout << "Empty Heap! Nothing to display!" << endl;
      return;
   }

   //display the right branch of the tree from the node (if exists)
   int child = right(node);
   if (child != -1)
      showTree(child, offset + 1);

   //display the current node at appropriate spacing
   for (int i = 0; i < offset; i++)
      cout << "\t";
   cout << list[node] << endl;

   //display the left branch of the tree from the node (if exists)
   child = left(node);
   if (child != -1)
      showTree(child, offset + 1);
}

//print the heap in the array format
void minHeap::printHeap()
{
   for (int i = 0; i < size; i++)
      cout << list[i + 1] << " ";
   cout << endl;
}

//decrease the key of a given index to new key
void minHeap::decreaseKey(int index, int newKey)
{
   //check for boundary conditions
   if (index < 1 || index > size)
      return;

   //if new key is higher, return
   if (newKey >= list[index])
      return;

   //else update to new key and siftUp to correct any distortions
   list[index] = newKey;
   siftUp(index);
}

//prints the menu for user
void printMenu()
{
   cout << "Menu:" << endl;
   cout << "1. Create New Heap" << endl;
   cout << "2. Insert an element in the heap" << endl;
   cout << "3. Delete the mininum element" << endl;
   cout << "4. Display sorted list (Heapsort)" << endl;
   cout << "5. Display in tree like form" << endl;
   cout << "6. Display the minimum element" << endl;
   cout << "7. Display the size of heap" << endl;
   cout << "8. Delete the entire heap" << endl;
   cout << "9. Print the heap" << endl;
   cout << "10. Decrease Key" << endl;
   cout << "11. Exit" << endl;
   cout << "Enter your choice: ";
}