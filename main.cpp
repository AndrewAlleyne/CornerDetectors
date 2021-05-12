#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class boundaryPt{
public:
    int x;
    int y;
    double curvature;
    int localMax;
    int corner;

};
class kCurvature{
    public:
        int K;
        int numPts;
        int Q;
        int P;
        int R;
        boundaryPt* PtAry;
        int neigh[5];




        kCurvature(int k_Curvature){
            this->K = k_Curvature;
        }

    void storePt(int x, int y, int index) {
            //Store into pointer array
            PtAry[index].x = x;
            PtAry[index].y = y;
    }

    //Begin
    void initialization(fstream &inputFile) {


        string line;
            //count the number of points in the inputFile
            while(getline(inputFile,line)) {
                numPts++;
            }
        PtAry = new boundaryPt [numPts];


            inputFile.flush();
            inputFile.clear();
            inputFile.seekp(14,inputFile.beg);

            int index = 0;
            while(index != numPts){
                int x;
                int y;

                inputFile >> x;
                inputFile >> y;

                storePt(x,y, index);
                index++;
            }
    }

    void printArray(ofstream &write) {
            //Display contents of array
        for (int i = 0; i < numPts; i++)
        {
            write << PtAry[i].x << " " <<  PtAry[i].y << endl;
        }
    }



    void cornerDetection(ofstream &of) {
            Q = 0;
            P = K;
            R = 2 * K;
            int index = P;

                int c;

            while(P != K - 1){

                c =  computeCurvature(Q,P,R);
                PtAry[index].curvature = c;

                of << "Q:" << Q << "  P:" << P <<  "  R:" << R << " index:" << index << " x:" << PtAry[index].x << "  y:" <<
                PtAry[index].y << "  curvature:" << PtAry[index].curvature <<  endl;

                Q = ((Q + 1) % numPts);
                P = ((P + 1) % numPts);
                R = ((R + 1) % numPts);
                index = ((index + 1)  % numPts);
            }

    }

    int computeCurvature(int q, int p, int r) {
            //look at the points and use K to get the next point.
            int qRow = PtAry[q].x;  //r1
            int qCol = PtAry[q].y;  //c1


            int pRow = PtAry[p].x;  //r2
            int pCol = PtAry[p].y;  //c2

            int rRow = PtAry[r].x;  //r3
            int rCol = PtAry[r].y;  //c3

            int c1_c2 = qCol - pCol;
            int r1_r2 = (qRow - pRow);

            if(r1_r2 == 0){
                r1_r2 = 1;
            }

            int c2_c3 = pCol - rCol;
            int r2_r3 = pRow - rRow;

            if(r2_r3 == 0){
                r2_r3 = 1;
            }

            double line1 = abs((c1_c2) / (r1_r2));
            double line2 =  abs((c2_c3) / (r2_r3));

            double  pCurvature = abs(line1 - line2);

        return pCurvature;
    }

    void localMaxima() {

        for(int i = 2; i < numPts; i++){
            findMaxima(i);
        }
    }

    void findMaxima(int i) {
        neigh[0] = PtAry[i - 2].curvature;
        neigh[1] = PtAry[i - 1].curvature;
        neigh[2] = PtAry[i].curvature;
        neigh[3] = PtAry[i + 1].curvature;
        neigh[4] = PtAry[i + 2].curvature;
        int currentPoint;


    }

    void markCorner() {
        for(int i = 0; i <= numPts - 1 ; i++){
            if(PtAry[i].localMax == 1 && PtAry[i-1].localMax == 1 ||PtAry[i+1].localMax == 1){
                PtAry[i].corner = 9;
            }else{
                PtAry[i].corner = 1;
            }
        }
    }

    void printBoundary(ofstream &of) {
        for(int i = 0; i < numPts; i++){
            of << " x:" << PtAry[i].x  << " y:" << PtAry[i].y << " corner:" <<  PtAry[i].corner << endl;
        }
    }

    void display(int ** imageArray) {
      for(int i = 0; i < numPts; i++){
          imageArray[PtAry[i].x][PtAry[i].y] = PtAry[i].corner;

      }
    }

    void prettyPrint(ofstream &of, int r, int c, int **array) {
            for( int i = 0; i < r; i++){
                for( int j = 0; j < c; j++){
                    if(array[i][j] == 0){
                        of << ". ";
                    }else{
                        of << array[i][j] << " ";
                    }
                }
                of << endl;
            }
    }
};

int main(int argc, char *argv[]) {

    //Console arguments.
    string dataFile = argv[1];
    string outFile = argv[2];
    string outFile1 = argv[3];
    string outFile2 = argv[4];

    //InputFile values
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int label;

    //Dynamic array for corner values
    int** imageAry;




    //User console input
    int k_Curvature;
    cout << "Please enter a K to be used in the K-curvature computation :";

    while( !(cin >> k_Curvature)){
        cin.clear();
        cin.ignore();
        cout << "K should be an number. Please enter a number." << endl;
    }

    //Obtain user input of K
    if(argc < 5)
    {
        cout << "You are missing the [K] curvature value. Please enter!" << endl;
    }

    //Open input file stream
    fstream inputFile;
    inputFile.open("data.txt");

    inputFile >> numRows >> numCols >> minVal >> maxVal >> label;


    imageAry = new int *[numRows];
    for(int row = 0; row < numRows; row++){
        imageAry[row] = new int[numCols];
    }

    for(int i = 0; i < numRows; i++){
        for(int j = 0; j < numCols; j++){
                imageAry[i][j] = 0;
        }
    }
    //Open output file stream
    ofstream outputFile_1 (outFile);
    ofstream outputFile_2 (outFile1);
    ofstream outputFile_3 (outFile2);

    //K curvature class
    kCurvature kC(k_Curvature);

    kC.initialization(inputFile);
    kC.printArray(outputFile_3);
    kC.cornerDetection(outputFile_3);
    kC.localMaxima();
    kC.markCorner();
    kC.printBoundary(outputFile_1);
    kC.display(imageAry);
    kC.prettyPrint(outputFile_2, numRows, numCols,imageAry);

    inputFile.close();
    outputFile_1.close();
    outputFile_2.close();
    outputFile_3.close();
    return 0;
}
