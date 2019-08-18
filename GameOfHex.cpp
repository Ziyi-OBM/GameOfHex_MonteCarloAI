
#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h> 
#include <time.h>       
#include <fstream>
#include <iterator>
#include <numeric>
#include <math.h> 
#include <random>


class Graph{
		
	
	public:
	//Parameters:
	
	
	//Default constructor
	Graph():V(1),rowg(V),G(V,rowg),rowe(V,0.0),E(V,rowe){	
		std::cout << "Default constructor called" << std::endl;
	}

	//Parameterized constructor
	explicit Graph(int ver):V(ver),rowg(V),G(V,rowg),rowe(V,0.0),E(V,rowe){}
	
	//Read File cnstructor	
	Graph(std::ifstream *inFile){	
		std::cout << "Read constructor called"  << std::endl;
		std::vector<int> inGraph;
		int val;
		while (*inFile >> val)
		{
			inGraph.push_back(val);
		}
		
		V=inGraph[0];
		int edgeNum=inGraph.size()-1;
		
		//Initialize a VxV bool matrix to represent the nodes
		rowg = std::vector<bool>(V);
		G = std::vector< std::vector<bool> >(V,rowg);

		//Initialize a VxV double matrix to store the cost of the edges
		rowe = std::vector<double>(V,0.0);
		E = std::vector< std::vector<double> >(V,rowe);	

		for (int i=1 ; i < edgeNum-3; i=i+3){
			addEdge (inGraph[i], inGraph[i+1], inGraph[i+2]);			
		}		
		
	}

	
	

	friend std::ostream& operator<< (std::ostream& out, const Graph& g);
	
	//Function to randomly initialize the graph 
	void generateRanomGraph(double edgeDensity, double distRangeLb, double distRangeUb) { 
		std::cout << "Randomly generating graph of density: " << edgeDensity << " and distance range: " << distRangeLb <<" ~ "<< distRangeUb <<std::endl; 
		srand(clock());   
		// srand(20190612);  Setting a constant seed for debugging
		if (distRangeLb>distRangeUb){
			std::cout << "Invalidd Distance Bounds" << std::endl;
			return;
		}
		double range = distRangeUb - distRangeLb;
		  for (int i=0;i<V;++i){
			for (int j=i;j<V;++j){
					if(i==j)G[i][j]=false;
						else{
							G[i][j]=G[j][i]=(prob()<edgeDensity);
							//if(G[i][j])E[i][j]=E[j][i]=(distRange*(1-prob()));
							if(G[i][j])E[i][j]=E[j][i]=(range*prob()+distRangeLb);
						}
			}
		  }	  
		
		//Print the resulting array to debug
		//disp(G);
		//std::cout << "\n" << std::endl;
		//disp(E);	   
	} 

	//Interface methods
	void printNodes() const {disp(&G);}	//Print the Nodes as a matrix

	void printEdges() const {disp(&E);}	//Print the Edges as a matrix

	int vertices() const {return V;} //Returns the number of vertices in the graph

	int edges() const {return count(&E);}	//Returns the number of edges in the graph

	bool adjacent(int i, int j) const {return G[i][j];}	//Tests whether there is an edge from node i to node j.

	std::vector<int> neighbors(int i) const {	//Print all nodes j such that there is an edge from i to j.
		std::vector<int> neighborNodes;
		for (int j=0;j<V;++j){
			if(G[i][j]){
				neighborNodes.push_back(j);
			}
		}
		return neighborNodes;
	}

	void addEdge (int i, int j, double cost){	//adds to G the edge from i to j with cost x, if it is not there.
			if(!G[i][j]){
				G[i][j]=G[j][i]=true;
				E[i][j]=E[j][i]=cost;
			}//else{std::cout<<"Error: Edge exists. The value is unchanged. Use setEdgeValue to assign new value"<<std::endl;}
	}

	void deleteEdge (int i, int j){	//removes the edge from i to j, if it exists.
			if(G[i][j]){
				G[i][j]=G[j][i]=false;
				E[i][j]=E[j][i]=0.0;
			}else{std::cout<<"Warning: Edge does not exists. No deletion peformed"<<std::endl;}
	}

	bool getNodeValue(int i, int j) const {return G[i][j];}	//returns the value associated to the node (i,j) (True/False=there Is/Not a node).

	// set_node_value is not necessary because its the same as the "addEdge" method

	double getEdgeValue(int i, int j) const {return E[i][j];}	//returns the value associated to the edge (i,j).

	double setEdgeValue(int i, int j, double cost){	//sets the value associated to the edge (i,j) to v.
		//If the two nodes are not connected, remind the user to use "addEdge" to add a new connection between the two nodes
		if(!G[i][j])std::cout<<"Error: No edge exists between given nodes. please use the 'addEdge' method"<<std::endl;
						else{E[i][j]=E[j][i]=cost;}
	}
			
		
	//Destructor
	~Graph() {} 


	//Private variables
	protected:
	
	inline double prob(){return (rand() / ((double)RAND_MAX+1));}
	
	//Changed to pass by reference
	template<typename T>
	inline void disp(T *vec2D) const {	//Print vector borrowed from stack overflow
		for(const auto &vec : *vec2D){
			for(const auto &x : vec)
				std::cout<<x<<" , ";
			std::cout << std::endl;
		}
	}
	
	//Changed to pass by reference
	template<typename T>
	inline int count(T *vec2D) const {
		int sum = 0;
		for(const auto &vec : *vec2D)
		{
			for(const auto &x : vec)
				if(x!=0.0)++sum;
		}
		return sum;
	}

	int V;	//# of vertices	
	//Initialize a VxV bool matrix to represent the nodes
	std::vector<bool> rowg;
	std::vector<std::vector<bool> >   G;

	//Initialize a VxV double matrix to store the cost of the edges
	std::vector<double> rowe;
	std::vector<std::vector<double> >  E;

};

std::ostream& operator<< (std::ostream& out, const Graph& g){
	std::cout << "Graph Object: "  << std::endl;
	std::cout << "Node Matrix: "  << std::endl;
	for(auto vec : g.G){
			for(auto x : vec)
				std::cout<<x<<" , ";
			std::cout << std::endl;
		}
		
	std::cout << "Edge Matrix: "  << std::endl;
	for(auto vec : g.E){
			for(auto x : vec)
				std::cout<<x<<" , ";
			std::cout << std::endl;
		}	
		
	return out;
}



class unionFind{
	protected:
	std::vector<int> arrRoots;
	std::vector<int> arrSize;
	int arrmax;
	
	public:

	// unionFind(int brdsize):arrRoots(brdsize),arrSize(brdsize),arrmax(brdsize){}
	//Initialize the arr to have size of the number of nodes
	unionFind(int tabSize):arrRoots(tabSize),arrSize(tabSize,1),arrmax(tabSize){
		for (int i=0;i<arrmax;i++) {
			arrRoots[i]=i;	
		}
		
	}
	
	
	int findRoot(int inPt){
		int root=inPt;
		
		while (root!=arrRoots[root]){
			root=arrRoots[root];
		}
		
		if (arrRoots[inPt]!=root) arrRoots[inPt]=root;
		return root;
	}
	
	int unionOp(int setA, int setB){
		if (findRoot(setA)!=findRoot(setB)){
			if (arrSize[findRoot(setA)]>=arrSize[findRoot(setB)]){
				arrRoots[findRoot(setB)]=findRoot(setA);
				arrSize[findRoot(setA)]+=arrSize[findRoot(setB)];
			}else{
				arrRoots[findRoot(setA)]=findRoot(setB);
				arrSize[findRoot(setB)]+=arrSize[findRoot(setA)];
			}
			return 1;//Indicate successfully unioned setA and setB
		}else{
			return 0;//Indicate setA and setB are already union
		}	
		
	}
	
	void reset(){
		for (int i=0;i<arrmax;i++) {
			arrRoots[i]=i;	
			arrSize[i]=1;	
		}
	}
	
	

};


enum class boardCell {empty = 0, red = 1, blue = 2}; 

class hexBoard: public Graph{
private:
	
protected:


	std::vector<boardCell> stones;
	int boardSize;
	int eptyCell;
	int trials=1000;
	unionFind mergeStones;
	
	
public:
	
	
	hexBoard(int hsize):Graph(hsize*hsize),boardSize(hsize),stones(hsize*hsize,boardCell::empty)
						,eptyCell(hsize*hsize),mergeStones(hsize*hsize){}
	
	inline int toListIdx(int r, int c){return r*boardSize+c;}
	
	
	void initBoard(){

		
		for (int i=0;i<V;i++){
			int row = i/boardSize;
			int col = i%boardSize;
			
			if(row==0 && col==0){
			addEdge (toListIdx(row, col), toListIdx(row+1, col), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row, col+1), 1.0);
			
			}else if(row==0 && col == boardSize-1){ 
			addEdge (toListIdx(row, col), toListIdx(row+1, col), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row, col-1), 1.0);
			
			}else if(row==boardSize-1 && col == 0){ 
			addEdge (toListIdx(row, col), toListIdx(row-1, col), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row, col+1), 1.0);
			
			}else if(row==boardSize-1 && col == boardSize-1){ 
			addEdge (toListIdx(row, col), toListIdx(row-1, col), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row, col-1), 1.0);
			
			}else if(row==0){ 
			addEdge (toListIdx(row, col), toListIdx(row, col+1), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row, col-1), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row+1, col-1), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row+1, col), 1.0);
			
			}else if(row==boardSize-1){ 
			addEdge (toListIdx(row, col), toListIdx(row, col+1), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row, col-1), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row-1, col), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row-1, col+1), 1.0);
			
			}else if(col==0){ 
			addEdge (toListIdx(row, col), toListIdx(row-1, col), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row+1, col), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row-1, col+1), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row, col+1), 1.0);
			
			}else if(col==boardSize-1){ 
			addEdge (toListIdx(row, col), toListIdx(row-1, col), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row+1, col), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row, col-1), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row+1, col-1), 1.0);
			
			}else{
			addEdge (toListIdx(row, col), toListIdx(row, col-1), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row, col+1), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row-1, col), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row+1, col), 1.0);	
			addEdge (toListIdx(row, col), toListIdx(row+1, col-1), 1.0);
			addEdge (toListIdx(row, col), toListIdx(row-1, col+1), 1.0);			
			}
			
			
		}	
	}
	
	int getBoardSize() const {return boardSize;}
	void setMcTrials(int i){trials=i;}
	void printWinner(){
		if (isWinner(boardCell::red)){std::cout<<"\n======O WINS======\n\n"<<std::endl;}
		else if(isWinner(boardCell::blue)){std::cout<<"\n======X WINS======\n\n"<<std::endl;}
		else{std::cout<<"NOT YET!!"<<std::endl;}
	}
	
	
	boardCell getStone(int node) const {return stones[node];}
	
	void printBoard(){
		//Note that O is enum(red), X is enum(blue);
		std::cout << "\nPlayer O: Left <-> Right"<<std::endl;
		std::cout << "Player X: Top <-> Bottom"<<std::endl;
		
		/*
		Test printing moves

		*/
		
		for (int i=0;i<2*boardSize;i++){
			if(i==0){	//Header
				for (int j=0;j<boardSize;j++){
				if (j+1<9)
					std::cout << "  "<< j+1<<" ";		//Print column number
				else
					std::cout << "  "<< j+1;		//Offset for the second digit
				}
			//Odd line prints slots
			}else if(i%2==1){
				if ((i+1)/2<10)
					std::cout << std::string(i-1, ' ') << (i+1)/2 << " ";	//Print row number
				else
					std::cout << std::string(i-2, ' ') << (i+1)/2 << " ";	//Offset for the second digit
				
				for (int j=0;j<boardSize;j++){
					
					if (j==boardSize-1){
						//Print the moves of red and blue player
						if (stones[toListIdx((i-1)/2,j)]==boardCell::red)
						std::cout << "O";
						else if (stones[toListIdx((i-1)/2,j)]==boardCell::blue)
						std::cout << "X";
						else
						std::cout << ".";
						
					}else{
						//Print the moves of red and blue player
						if (stones[toListIdx((i-1)/2,j)]==boardCell::red)
						std::cout << "O";
						else if (stones[toListIdx((i-1)/2,j)]==boardCell::blue)
						std::cout << "X";
						else
						std::cout << ".";
					
					std::cout << " - ";
					}		
				}
			//Even line prints connections
			}else if(i%2==0){
				std::cout << std::string(i+1, ' ');	//Print row number
				for (int j=0;j<boardSize;j++){
					if (j==boardSize-1){
						std::cout << "\\";
					}else{
						std::cout << "\\ / ";
					}						
				}
			}
			std::cout << std::endl;	
		}
	}
	
	int makeMove(int const row, int const col, boardCell const player){
		int idx = toListIdx(row, col);	
		
		if (static_cast<int>(stones[idx])){
			std::cout <<"occupied Cell. Invalid Move."<< std::endl;
			return 0;
		}
		switch (player){
			case boardCell::red: 
				stones[idx]=player;
				eptyCell--;
				break;
				
			case boardCell::blue: 
				stones[idx]=player;
				eptyCell--;
				break;
				
			default: 
				std::cout <<"Invalid input"<< std::endl;
				return 0;
			// code to be executed if n doesn't match any cases
		}
		return 1;	
	}
	

	
	int isWinner(boardCell player) {
		
		
		mergeStones.reset();
		
		for (int i=0;i<V;i++){
			for(int j=0;j<i;j++){
				if (getEdgeValue(i,j)){
					if (getStone(i)!=boardCell::empty && getStone(i)==getStone(j))
					mergeStones.unionOp(i,j);
				}
			}
		}
		
		//create a lookup table, essentially a hash map.
		std::vector<int> lookupTab(boardSize*boardSize,0);
		
		switch (player){
			case boardCell::red: 
			//Insert the roots of the left edge nodes into the lookup table as keys
				//Loop over the left edge
				for (int i=0; i<boardSize; i++){
					if (getStone(i*boardSize)==boardCell::red)
						lookupTab[mergeStones.findRoot(i*boardSize)]=1;
				}
			//Check if the roots of the right edge nodes are in the lookup table	
				//Loop over the right edge
				for (int i=1; i<=boardSize; i++){
					//If theres a red stone, and its root is in the lookup table
					if (getStone(i*boardSize-1)==boardCell::red && lookupTab[mergeStones.findRoot(i*boardSize-1)])
					//return true
					return 1;
				}
				return 0;
				
				break;
	
			case boardCell::blue: 	
			//Insert the roots of the top edge nodes into the lookup table as keys
				//Loop over the top edge
				for (int i=0; i<boardSize; i++){
					if (getStone(i)==boardCell::blue)
						lookupTab[mergeStones.findRoot(i)]=1;
				}
			//Check if the roots of the bottom edge nodes are in the lookup table	
				//Loop over the bottom edge
				for (int i=boardSize*(boardSize-1); i<boardSize*boardSize; i++){
					//If theres a red stone, and its root is in the lookup table
					if (getStone(i)==boardCell::blue && lookupTab[mergeStones.findRoot(i)])
					//return true
					return 1;
				}
				return 0;				
				break;
				
				
			default: 
				std::cout <<"Invalid player id"<< std::endl;
				return -1;
			// code to be executed if player doesn't match any cases
		}
		
		
	}
	
	
	double mcEval(int move, boardCell player){
		
		//static auto rng = std::default_random_engine {};
		int win=0;
		int numRed;
		int numBlue;
		std::vector<int> emptyIdx;	//keep a record of which cells on the board are empty
		
		for (int i=0;i<stones.size();i++){
				if (stones[i]==boardCell::empty && i!=move){
					emptyIdx.push_back(i);
				}
			}
		
		//Make the move that will be evaluated
		if (stones[move]==boardCell::empty){
			stones[move]=player;
		}else{
			std::cout<<"Bug: evaluating occupied board cell"<<std::endl;
			return -1.0;
		}
		
		
		if (player == boardCell::red){
				numRed = floor((eptyCell-1)/2.0);
				numBlue = ceil((eptyCell-1)/2.0);
			}else if (player == boardCell::blue){
				numRed = ceil((eptyCell-1)/2.0);
				numBlue = floor((eptyCell-1)/2.0);
		}
		
		//std::vector<boardCell> moveShuffle(numRed+numBlue);
		std::vector<int> moveShuffle(numRed+numBlue);
		
		
		for (int it=0;it<trials;it++){	

			for (int i=0;i<numRed;i++){
				//moveShuffle[i]=boardCell::red;
				moveShuffle[i]=1;
			}
			for (int i=0;i<numBlue;i++){
				//moveShuffle[numRed+i]=boardCell::blue;
				moveShuffle[numRed+i]=2;
			}

			std::random_shuffle(moveShuffle.begin(), moveShuffle.end()); 		
			//http://www.cplusplus.com/reference/algorithm/shuffle/
			//std::shuffle ( moveShuffle.begin(), moveShuffle.end(),  rng);
			

			for (int i=0;i<emptyIdx.size();i++){
				stones[emptyIdx[i]]=static_cast<boardCell>(moveShuffle[i]);
			}

			if (isWinner(player)==1) {win++;}

			
		}
		
		//Reset the board to original state
		stones[move]=boardCell::empty;
		for (auto x:emptyIdx){
			stones[x]=boardCell::empty;
		}
			
		return static_cast<double>(win)/trials	;	
	}
	
	
	int aiMove(boardCell aiColor){
		
		std::cout << "Computer move: \nCalculating...(Takes 10~20s)" <<std::endl;
		
		
		std::vector<int> possibleMoves;	//keep a record of which cells on the board are empty
		
		for (int i=0;i<stones.size();i++){
			if (stones[i]==boardCell::empty){
				possibleMoves.push_back(i);
			}
		}
		
		std::vector<double> mcScore;

		for(auto x:possibleMoves){
			//std::cout << "Evaluating: " << x <<std::endl;
			mcScore.push_back(mcEval(x, aiColor));
		}
		
		int maxIndex = possibleMoves[std::max_element(mcScore.begin(),mcScore.end()) - mcScore.begin()];		
		
		makeMove(maxIndex/boardSize , maxIndex%boardSize, aiColor);
		
	}
	
	
	
	~hexBoard() {} 
};






int main(){
	

	srand(clock());
	hexBoard test(11);
	test.initBoard();
	test.setMcTrials(1000);
	
	int a, b;
	int pFirst;
	int validMove=0;
	
	/*
	std::cout<<"Do you want to start first ? [Yes=1/No=0]";
	std::cin >> pFirst;
	while ( (pFirst!=0) && (pFirst!=1)){
		std::cout<<"Invalid Input. [Yes=1/No=0]";
		std::cin >> pFirst;
	}
	
	switch (pFirst){
		case 1: 
			//Player make move block
			while (!validMove){
				validMove=test.makeMove(a-1,b-1,boardCell::red);
				if (!validMove){
					std::cout << "Invalid Move. Try again...\nEnter row and column:"<<std::endl;
					std::cin >> a >> b;
				}
			}；
			
			test.mcEval(test.toListIdx(a-1,b-1), boardCell::red);
			
			break
		case 0: 
			break
		default:
			break
	}	
	*/	

	//Start turns
	
	while (1){
		test.printBoard();
		
		std::cout << "Your Turn: Enter row and column:"<<std::endl;
		std::cin >> a >> b;
		
		//-1 to count for the 0 indexing
		while (!validMove){
			validMove=test.makeMove(a-1,b-1,boardCell::red);
			if (!validMove){
				std::cout << "Invalid Move. Try again...\nEnter row and column:"<<std::endl;
				std::cin >> a >> b;
			}
		}
		validMove=0;
		
		test.printBoard();
		if (test.isWinner(boardCell::red)){break;}
		
		test.aiMove(boardCell::blue);
		if (test.isWinner(boardCell::blue)){test.printBoard(); break;}
		
	}

	test.printWinner();



	//int retry=1;
	//while (retry){
		
		//std::cout << "Try again? \n";
		//std::cin >> retry;

	//}

return 0;
}
