#include <iostream>
#include <new>
#include <cstdio>
#include <fstream>
#include <iomanip>

#define NSIZE 3
#define sumN 30
#define A (int)7
#define B (int)6
#define C (int)5
#define D (int)4

using namespace::std;

void print(int matrix[sumN][sumN],int** V, int* NE, int Nseize, ofstream& fout){
    //for(int k=0;k<sumN;k++){
        for(int i=0; i < Nseize; i++){
            for(int j=0; j < NE[i]; j++){
                for(int m=0; m < Nseize; m++){
                    for(int n=0; n < NE[m]; n++){
                    cout<<matrix[V[i][j]][V[m][n]]<<" ";
                    fout<<matrix[V[i][j]][V[m][n]]<<" ";
                    }
                    fout<<" | ";
                }
        cout<<endl;
        fout<<endl;
        }
        fout << "--------------------------------------------------------------------------"<<endl;
    }
}

int findQ(int matrix[sumN][sumN],int** V, int* NE, int Nseize){
    int Q=0;
    bool flagi, flagj,bigflag;
    for(int i=0; i < sumN; i++)
        for(int j=0; j < sumN; j++)
            for(int k=0; k < Nseize; k++){
                flagi = flagj = false;
                for(int m=0;m < NE[k]; m++){
                    if(V[k][m] == i)
                        flagi = true;
                    if(V[k][m] == j)
                        flagj = true;
                }
                if(flagi && flagj)
                    break;
                if(flagi || flagj){
                    Q += matrix[i][j];
                    break;
                }
            }
    cout << "Q = " << Q/2 << endl;
    return Q/2;
}

int* summMatrix(int* testSum,int** matrixHelp, int testSumN, int* exclude, ofstream& fout){
    int minTSum = 100000;
    int minIndex = -1;
    bool flag;
    static int ret[2];
    for(int i=0;i<testSumN;i++){
        flag = true;
        for(int j=0;j<testSumN;j++){
        fout << matrixHelp[i][j] << " ";
            testSum[i]+=matrixHelp[i][j];
        }
        fout << "| "<<testSum[i] <<endl;
        if(testSum[i] < minTSum){
            for(int k=0; (exclude[k] != -1) && flag;k++)
                if(exclude[k] == i)
                    flag = false;
            if(flag){
                minTSum = testSum[i];
                minIndex = i;
            }
        }
    }
    ret[0] = minIndex;
    ret[1] = minTSum;
   // fout << "sum"<<endl;
   // for(int i =0;i<testSumN;i++)
     //   fout << testSum[i] <<endl;
    return ret;
}


int main(){
    int matrix[30][30] = {
                            {0, 0, 2, 1, 3, 0, 3, 4, 0, 4, 0, 2, 2, 2, 2, 2, 2, 0, 0, 3, 0, 0, 0, 0, 0, 4, 2, 0, 2, 3},
                            {0, 0, 0, 0, 1, 0, 2, 0, 1, 0, 0, 4, 0, 3, 0, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 2},
                            {2, 0, 0, 0, 2, 0, 1, 0, 4, 2, 0, 2, 0, 2, 0, 1, 0, 0, 2, 4, 0, 4, 0, 4, 1, 0, 0, 0, 2, 3},
                            {1, 0, 0, 0, 1, 3, 1, 4, 4, 2, 0, 4, 3, 0, 0, 1, 0, 1, 1, 0, 0, 0, 2, 3, 0, 0, 1, 0, 0, 0},
                            {3, 1, 2, 1, 0, 4, 2, 3, 4, 2, 0, 0, 4, 0, 0, 1, 2, 0, 3, 2, 2, 3, 0, 1, 0, 0, 0, 4, 0, 2},
                            {0, 0, 0, 3, 4, 0, 2, 0, 0, 1, 3, 0, 4, 0, 1, 1, 3, 2, 3, 0, 0, 2, 4, 3, 0, 1, 2, 0, 0, 4},
                            {3, 2, 1, 1, 2, 2, 0, 0, 3, 1, 3, 0, 0, 0, 0, 1, 0, 3, 0, 0, 1, 1, 0, 2, 1, 0, 3, 0, 1, 0},
                            {4, 0, 0, 4, 3, 0, 0, 0, 0, 0, 0, 0, 4, 3, 0, 0, 0, 1, 2, 4, 3, 4, 3, 0, 0, 0, 3, 1, 3, 0},
                            {0, 1, 4, 4, 4, 0, 3, 0, 0, 3, 3, 0, 0, 2, 2, 4, 0, 0, 4, 0, 3, 1, 0, 3, 4, 0, 0, 0, 1, 0},
                            {4, 0, 2, 2, 2, 1, 1, 0, 3, 0, 4, 0, 4, 0, 2, 0, 0, 1, 0, 0, 3, 0, 2, 0, 2, 0, 1, 0, 0, 0},
                            {0, 0, 0, 0, 0, 3, 3, 0, 3, 4, 0, 4, 0, 0, 0, 0, 3, 0, 0, 0, 1, 0, 3, 4, 0, 0, 0, 0, 4, 0},
                            {2, 4, 2, 4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 2, 0, 0, 1, 2, 2, 0, 0, 3, 2, 2, 3, 0, 0},
                            {2, 0, 0, 3, 4, 4, 0, 4, 0, 4, 0, 0, 0, 0, 3, 0, 1, 0, 1, 4, 2, 4, 0, 0, 1, 0, 4, 0, 0, 0},
                            {2, 3, 2, 0, 0, 0, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 2, 0, 2, 1, 0, 1, 0, 1, 1, 0, 0, 3, 0, 4},
                            {2, 0, 0, 0, 0, 1, 0, 0, 2, 2, 0, 0, 3, 0, 0, 0, 2, 0, 0, 2, 2, 0, 0, 0, 4, 0, 0, 3, 2, 0},
                            {2, 0, 1, 1, 1, 1, 1, 0, 4, 0, 0, 0, 0, 0, 0, 0, 2, 1, 4, 2, 2, 0, 1, 0, 3, 4, 1, 0, 3, 3},
                            {2, 0, 0, 0, 2, 3, 0, 0, 0, 0, 3, 2, 1, 2, 2, 2, 0, 3, 0, 3, 0, 3, 4, 4, 1, 0, 1, 3, 4, 1},
                            {0, 0, 0, 1, 0, 2, 3, 1, 0, 1, 0, 0, 0, 0, 0, 1, 3, 0, 0, 1, 2, 0, 0, 0, 2, 2, 0, 0, 3, 4},
                            {0, 0, 2, 1, 3, 3, 0, 2, 4, 0, 0, 0, 1, 2, 0, 4, 0, 0, 0, 0, 0, 1, 0, 4, 1, 0, 0, 4, 0, 4},
                            {3, 0, 4, 0, 2, 0, 0, 4, 0, 0, 0, 1, 4, 1, 2, 2, 3, 1, 0, 0, 1, 4, 4, 0, 1, 3, 4, 3, 0, 3},
                            {0, 2, 0, 0, 2, 0, 1, 3, 3, 3, 1, 2, 2, 0, 2, 2, 0, 2, 0, 1, 0, 3, 1, 4, 1, 0, 1, 1, 3, 4},
                            {0, 0, 4, 0, 3, 2, 1, 4, 1, 0, 0, 2, 4, 1, 0, 0, 3, 0, 1, 4, 3, 0, 2, 0, 3, 0, 0, 4, 1, 0},
                            {0, 0, 0, 2, 0, 4, 0, 3, 0, 2, 3, 0, 0, 0, 0, 1, 4, 0, 0, 4, 1, 2, 0, 3, 0, 0, 1, 4, 3, 0},
                            {0, 2, 4, 3, 1, 3, 2, 0, 3, 0, 4, 0, 0, 1, 0, 0, 4, 0, 4, 0, 4, 0, 3, 0, 0, 2, 1, 3, 2, 0},
                            {0, 0, 1, 0, 0, 0, 1, 0, 4, 2, 0, 3, 1, 1, 4, 3, 1, 2, 1, 1, 1, 3, 0, 0, 0, 0, 3, 2, 1, 1},
                            {4, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 4, 0, 2, 0, 3, 0, 0, 0, 2, 0, 0, 4, 1, 4, 1},
                            {2, 2, 0, 1, 0, 2, 3, 3, 0, 1, 0, 2, 4, 0, 0, 1, 1, 0, 0, 4, 1, 0, 1, 1, 3, 4, 0, 4, 0, 4},
                            {0, 0, 0, 0, 4, 0, 0, 1, 0, 0, 0, 3, 0, 3, 3, 0, 3, 0, 4, 3, 1, 4, 4, 3, 2, 1, 4, 0, 0, 1},
                            {2, 0, 2, 0, 0, 0, 1, 3, 1, 0, 4, 0, 0, 0, 2, 3, 4, 3, 0, 0, 3, 1, 3, 2, 1, 4, 0, 0, 0, 4},
                            {3, 2, 3, 0, 2, 4, 0, 0, 0, 0, 0, 0, 0, 4, 0, 3, 1, 4, 4, 3, 4, 0, 0, 0, 1, 1, 4, 1, 4, 0}
                         };

    int **V, *vTestHelp, vTestHelpSum;
    int indexSize,*helparr,exclude[sumN];
    int index[sumN],delta[sumN],deltaR[A][sumN],testSum[sumN],vTest[sumN];;
    int i,j,k,l,m,n,iterNum;
    int minTSum, testSumN, minIndex, testSumIndex,maxIndex,maxHSum;
    int **matrixHelp;
    int **NE,*NSEIZE,combNum=0;
    int change[4],deltaRSize,rememberJ,rememberL,Q,Qlast,bestIter;
    bool flag;
    ofstream fout("itog.txt"); // создаём объект класса ofstream для записи и связываем его с файлом cppstudio.txt

    Qlast=0;
    for(i=0;i<sumN;i++)
        for(j=0;j<sumN;j++)
            Qlast += matrix[i][j];
    cout << Qlast << endl;
    //return 0;

    //комбинаторика
    for(i = (sumN/A);i>=0;i--)
        for(j = (sumN-i*A)/B;j>=0;j--)
            for(k=(sumN-i*A-j*B)/C;k>=0;k--)
                for(m=(sumN-i*A-j*B-k*C)/D;m>=0;m--)
                    if((i*A + j*B + k*C + m*D)==sumN)
                        combNum++;

    cout << combNum<<endl;
    NSEIZE = new int[combNum];
    NE = new int* [combNum];
    combNum = 0;
    for(i = sumN/A;i>=0;i--)
        for(j = (sumN-i*A)/B;j>=0;j--)
            for(k=(sumN-i*A-j*B)/C;k>=0;k--)
                for(m=(sumN-i*A-j*B-k*C)/D;m>=0;m--)
                    if((i*A + j*B + k*C + m*D)==sumN){
                        NSEIZE[combNum] = i+j+k+m;
                        NE[combNum] = new int [NSEIZE[combNum]];
                        for(n=0;n<NSEIZE[combNum];n++){
                            if(n<i)
                                NE[combNum][n] = A;
                            else if((n-i)<j)
                                NE[combNum][n] = B;
                            else if((n-i-j)<k)
                                NE[combNum][n] = C;
                            else if((n-i-j-k)<m)
                                NE[combNum][n] = D;
                        }
                        combNum++;
                    }
    for(i=0;i<combNum;i++){
        for(j=0;j<NSEIZE[i];j++)
            cout << NE[i][j] <<"    ";
        cout << "       NSIZE = "<< NSEIZE[i] << endl;
    }

    for(iterNum=0;iterNum<combNum;iterNum++){
        Q =0;
        cout <<"iteration  "<< iterNum+1 << endl;
        cout<<endl;
        fout <<"iteration  "<< iterNum+1 << endl;
        fout<<endl;
        //последовательный этап
        V = new int* [NSEIZE[iterNum]];
        if(V == NULL){
                cout << "i can't get memory" << endl;
            }
        for(i=0;i<NSEIZE[iterNum];i++)
            V[i] = new int[NE[iterNum][i]];
        testSumN = sumN;
        indexSize = sumN;
        for(i=0;i<sumN;i++)
            index[i] = i;

        for(k=0;k<(NSEIZE[iterNum]-1);k++){

            matrixHelp = new(int* [testSumN]);

            if(matrixHelp == NULL){
                cout << "i can't get memory" << endl;
            }
            for(int i=0;i<testSumN;i++)
                matrixHelp[i] = new(int [testSumN]);
            fout << "matrix "<<endl;
            for(int i=0;i<testSumN;i++){
                testSum[i] = 0;
                exclude[i] = -1;
                for(j=0;j<testSumN;j++){
                    matrixHelp[i][j] = matrix[index[i]][index[j]];
                   // fout << matrixHelp[i][j] << " ";
                }
                //fout << endl;
            }

            helparr = summMatrix(testSum,matrixHelp,testSumN,exclude,fout);
            minIndex = helparr[0];
            minTSum = helparr[1];
            fout << "min sum  "<<minTSum << endl;
            fout << "min index  "<<minIndex << endl;
            testSumIndex = 0;
            for(i=0;i<testSumN;i++)
                if(matrixHelp[minIndex][i])
                    testSumIndex++;
            if((testSumIndex+1) == NE[iterNum][k]){
                cout << "normal at start" <<endl;
                fout << "normal"<<endl;
                V[k][0] = index[minIndex];
                for(i=0,j=1;i<testSumN;i++)
                    if(matrixHelp[minIndex][i])
                        V[k][j++] = index[i];
                fout <<"V = { ";
                for(i=0;i<j;i++)
                    fout << V[k][i]+1 << " ";
                fout << "}" <<endl;
                cout << "V = { ";
                for(i=0;i<j;i++)
                    cout << V[k][i] << " ";
                cout << "}" <<endl;
                for(i=0;i<NE[iterNum][k];i++)
                    for(j=0;j<testSumN;j++)
                        if(V[k][i] == index[j]){
                            index[j] = index[indexSize-1];
                            indexSize--;
                            break;
                        }
             //   for(int i=0;i<indexSize;i++)
             //       cout << index[i] <<endl;
             //   cout <<endl;
                for(i=0;i<testSumN;i++)
                    delete matrixHelp[i];
                delete matrixHelp;
                matrixHelp = NULL;
                testSumN -=NE[iterNum][k];
                continue;
            }

            l=0;
            exclude[0] = vTest[l++] = minIndex;

            for(i=0;i<testSumN;i++)
                if(matrixHelp[minIndex][i])
                        vTest[l++] = i;
            fout <<"V index in matrix = { ";
            for(i=0;i<l;i++)
                fout << vTest[i] << " ";
            fout << "}" <<endl;
            if(l == 1)
                cout << "null" <<endl;
            while((minTSum == 0) && (l<NE[iterNum][k])){
                //cout << "null" <<endl;
                helparr = summMatrix(testSum,matrixHelp,testSumN,exclude,fout);
                minIndex = helparr[0];
                minTSum = helparr[1];
                exclude[l] = minIndex;
                vTest[l++] = minIndex;
            }

            n=0;
            if(testSumIndex < NE[iterNum][k]){
                fout << "include" << endl;
                cout << "include" <<endl;
                while(n<NE[iterNum][k]){
                    n=l;
                    for(i=0;i<l;i++){
                        for(j=0;j<testSumN;j++){
                            //fout << matrixHelp[vTest[i]][j] <<" ";
                            if(matrixHelp[vTest[i]][j]){
                                flag = false;
                                for(m=0;(m < n) && !flag;m++)
                                    if(j == vTest[m]){
                                        flag = true;
                                        break;
                                    }
                                if(!flag)
                                    vTest[n++] = j;
                            }
                        }
                        fout << endl;
                    }
                    for(i=0;i<testSumN;i++){
                        for(j=0;j<testSumN;j++)
                            fout<< matrixHelp[i][j] <<" ";
                        fout << endl;
                    }
                    fout<<"V index in matrix = { ";
                    for(i=0;i<n;i++)
                        fout<< vTest[i] <<" ";
                    fout<< "}"<<endl;
                    if(n>l) l = n;
                }
            }

            if(n==0)
                n = l;
            else n--;
            if(n > NE[iterNum][k]){
                cout << "exclude" <<endl;
                fout << "exclude" <<endl;
            }
            while(n > NE[iterNum][k]){
                //cout << "exclude" <<endl;
                maxHSum = 0;
                fout << "matrix_exclude"<<endl;
                for(i=0;i<n;i++){
                    delta[i]=testSum[vTest[i]];
                    fout << delta[i] << " - ( ";
                    for(j=0;j<n;j++){
                        fout << matrixHelp[vTest[i]][vTest[j]] << " ";
                        delta[i] -= matrixHelp[vTest[i]][vTest[j]];
                    }
                    fout <<") = "<<delta[i] << "(" << vTest[i] << ")" <<endl;
                    if(delta[i] > maxHSum) {
                            maxIndex = i;
                            maxHSum = delta[i];
                    }
                }

                vTest[maxIndex] = vTest[n-1];
                n--;
                fout<<"V index in matrix = { ";
                for(i=0;i<n;i++)
                    fout<<vTest[i]
                    <<" ";
                fout << "}" << endl;
            }

            for(i=0,j=0;i<testSumN;i++)
                if(j<NE[iterNum][k])
                    V[k][j++] = index[vTest[j]];
            fout<<"V = { ";
            for(i=0;i<j;i++)
                    fout<< V[k][i]+1 <<" ";
            fout<< "}"<<endl;
            for(i=0;i<NE[iterNum][k];i++)
                for(j=0;j<testSumN;j++)
                    if(V[k][i] == index[j]){
                        index[j] = index[indexSize-1];
                        indexSize--;
                        break;
                    }
          //  for(int i=0;i<indexSize;i++)
           //     cout << index[i] <<endl;
           // cout <<endl;
            for(i=0;i<testSumN;i++)
                delete matrixHelp[i];
            delete matrixHelp;
            matrixHelp = NULL;
            testSumN-=NE[iterNum][k];
            getchar();
        }

        fout<<"V = { ";
        for(i=0;i<NE[iterNum][k];i++){
            V[k][i] = index[i];
            fout<< V[k][i]+1 <<" ";
        }
        fout<< "}"<<endl;
        cout << endl;
        cout << "final " << endl;
        fout << "final " << endl;
        for(i=0;i<NSEIZE[iterNum];i++){
            fout<<"V[" << i+1 <<"] = { ";
            for(j=0;j<NE[iterNum][i];j++){
                cout << (V[i][j]+1) << endl;
                fout<< V[i][j]+1 <<" ";
            }
            fout<< "}"<<endl;
            cout << endl;
        }
        Q = findQ(matrix,V,NE[iterNum],NSEIZE[iterNum]);
        print(matrix,V,NE[iterNum],NSEIZE[iterNum], fout);
        fout << "Q = " << Q <<endl;

        //итерационный этап

        fout << "iteration" <<endl;
        deltaRSize = sumN;
        for(k=0;k < (NSEIZE[iterNum]-1);k++){
            maxHSum = 1;
            while(maxHSum){
                maxHSum=0;
                for(i=0;i<NE[iterNum][k];i++)
                    for(j=0;j<deltaRSize;j++)
                        deltaR[i][j] = 0;
                for(i=0;i<NE[iterNum][k];i++){
                    for(j=0;j<deltaRSize;j++){
                        flag = true;
                        for(m=0;m<(k+1);m++)
                            for(l=0;l<NE[iterNum][m];l++)
                                if(j == V[m][l])
                                    flag = false;
                        if(!flag)
                            continue;
                        for(m=0;(m<NSEIZE[iterNum]) && flag;m++){
                            for(l=0;(l<NE[iterNum][m]) && flag;l++)
                                if(V[m][l] == j){
                                    rememberJ = m;
                                    rememberL = l;
                                    flag = false;
                                    break;
                            }
                        }

                        for(m=0;m<NE[iterNum][rememberJ];m++)
                            deltaR[i][j] += matrix[V[k][i]][V[rememberJ][m]];
                        for(m=0;m<NE[iterNum][k];m++)
                            deltaR[i][j] -= matrix[V[k][i]][V[k][m]];
                        for(m=0;m<NE[iterNum][k];m++)
                            deltaR[i][j] += matrix[V[rememberJ][rememberL]][V[k][m]];
                        for(m=0;m<NE[iterNum][rememberJ];m++)
                            deltaR[i][j] -= matrix[V[rememberJ][rememberL]][V[rememberJ][m]];
                        deltaR[i][j] -= 2*matrix[V[k][i]][V[rememberJ][rememberL]];
                        fout <<setw(3)<< deltaR[i][j] << " ";
                        if(deltaR[i][j] > maxHSum){
                            maxHSum = deltaR[i][j];
                            change[0] = i;
                            change[1] = rememberJ;
                            change[2] = l;
                        }
                    }
                    fout << endl;
                }
                fout << "maxHSum = " << maxHSum << endl;
                if(maxHSum > 0){
                    fout << "change V["<<change[0] + 1<<"] real (" << V[k][change[0]]+1 <<") and V real(" << V[change[1]][change[2]]+1 <<")"<<endl;
                    change[3] = V[k][change[0]];
                    V[k][change[0]] = V[change[1]][change[2]];
                    V[change[1]][change[2]] = change[3];
                    Q = findQ(matrix,V,NE[iterNum],NSEIZE[iterNum]);
                }
                for(i=0;i<NSEIZE[iterNum];i++){
                    fout << "V[" << i+1 <<"] = { ";
                    for(j=0;j<NE[iterNum][i];j++)
                        fout << V[i][j]+1 << " ";
                    fout << "}" <<endl;
                }
                fout << "Q = " << Q <<endl;
            }
        }
        cout << "optimaze final" << endl;
        for(i=0;i<NSEIZE[iterNum];i++){
            for(j=0;j<NE[iterNum][i];j++)
                cout << (V[i][j]+1) << endl;
            cout << endl;
        }
        Q = findQ(matrix,V,NE[iterNum],NSEIZE[iterNum]);
        if(Q < Qlast){
            Qlast = Q;
            bestIter = iterNum;
        }
        print(matrix,V,NE[iterNum],NSEIZE[iterNum],fout);
        for(i=0;i<NSEIZE[iterNum];i++)
            delete V[i];
        delete V;
        V = NULL;
        getchar();
    }
    cout << "best Iteration " << bestIter << endl;
    fout.close();
    for(i=0;i<combNum;i++)
        delete NE[i];
    delete NE;
    delete NSEIZE;
    return 0;
}
