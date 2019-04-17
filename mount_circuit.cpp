#include <iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <fstream>
#include <iomanip>

#define N 30
#define USED -1138

using namespace std;

int findXY(vector<int> t,int Vi, int jSize, char c){
    vector<int>::iterator index;
    int realIndex;
    index = find(t.begin(),t.end(),Vi);
    realIndex = abs(distance(t.begin(),index));
    if(c == '/')
    return realIndex/jSize;
    else return realIndex%jSize;
}

int findL(vector<int> t,int Vi, int Vj, int jSize, char c){
    vector<int>::iterator index;
    int realIndex1,realIndex2;
    index = find(t.begin(),t.end(),Vi);
    realIndex1 = abs(distance(t.begin(),index));
    index = find(t.begin(),t.end(),Vj);
    realIndex2 = abs(distance(t.begin(),index));
    if(c == '/')
    return abs(realIndex1/jSize - realIndex2/jSize);
    else return abs(realIndex1%jSize - realIndex2%jSize);
}

int findVbad(int matrix[N*N],vector<int> t,int jSize, int ro[N], ofstream& fout){
    double Q1,Q2;
    int Vbad = -1;
    Q1 = Q2 = 0.0;
    fout<< "find Vbad" <<endl;
    for(int i=0;i<N;i++){
        Q1 = 0;
        for(int j=0;j<N;j++)
            if(matrix[i*N + j])
                Q1 += (double)(matrix[i*N + j]*(findL(t,i,j,jSize,'/') + findL(t,i,j,jSize,'%')));
        Q1 /= (double)ro[i];
        fout << "V[" << i << "] = " << Q1 << endl;
        if(Q1 > Q2){
            Q2 = Q1;
            Vbad = i;
        }
    }
    fout << "MOST BAD is V[" << Vbad << "]" << endl;
    return Vbad;
}

double findCenter(int matrix[N*N],vector<int> t,int jSize, int ro,int Vbad, char c){
    double coord = 0;
    for(int j=0;j<N;j++)
        if(matrix[Vbad*N + j])
            coord += (double)(matrix[Vbad*N + j]*findXY(t,j,jSize,c));
    coord /= (double)ro;
    return coord;
}

int findQ(int matrix[N*N],vector<int> t,int jSize){
    int Q=0;
    //cout<< "find Q:" <<endl;
    for(int i=0;i<N;i++)
        for(int j=(i+1);j<N;j++)
            if(matrix[i*N + j])
                Q += matrix[i*N + j]*(findL(t,i,j,jSize,'/')+findL(t,i,j,jSize,'%'));
    return Q;
}

bool compar(const int a, const int b){
    if(a >= 0)
        return true;
    if(b >= 0)
        return false;
    if(a < b)
        return true;
    else
        return false;
}

vector<int> pushV(vector<int> maxV, vector<int> t, int iSize){
    vector<int>::iterator rezult;
    for(auto i:maxV){
        rezult = max_element(t.begin(), t.end(), compar);
        t[distance(t.begin(),rezult)]=i;
    }
    return t;
}

int main(){
    int i,j,Q,Qlast,Vbad;
    double Xc,Yc;
    int matrix[N*N] =  {0, 2, 0, 0, 0, 0, 3, 0, 1, 0, 2, 0, 3, 4, 4, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 0, 2, 0, 0,
                        2, 0, 0, 4, 0, 0, 4, 0, 0, 3, 4, 0, 0, 1, 3, 1, 0, 3, 1, 0, 4, 3, 4, 4, 0, 4, 0, 4, 2, 0,
                        0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 4, 3, 1, 4, 4, 0, 3, 0, 0, 0, 0, 0, 2, 1, 0, 3,
                        0, 4, 0, 0, 1, 0, 0, 0, 2, 0, 1, 0, 0, 3, 2, 2, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 4, 0, 0,
                        0, 0, 0, 1, 0, 1, 4, 3, 0, 0, 1, 0, 0, 3, 2, 0, 0, 0, 4, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 1,
                        0, 0, 0, 0, 1, 0, 0, 2, 1, 1, 0, 0, 0, 2, 0, 0, 1, 0, 0, 2, 4, 0, 0, 2, 2, 1, 3, 0, 0, 0,
                        3, 4, 2, 0, 4, 0, 0, 2, 1, 0, 0, 0, 2, 1, 0, 0, 0, 3, 0, 3, 2, 0, 0, 1, 4, 4, 0, 4, 0, 0,
                        0, 0, 2, 0, 3, 2, 2, 0, 0, 0, 0, 3, 0, 4, 0, 0, 0, 0, 0, 0, 0, 3, 0, 4, 0, 1, 0, 4, 1, 0,
                        1, 0, 0, 2, 0, 1, 1, 0, 0, 2, 0, 0, 3, 3, 0, 0, 2, 0, 3, 2, 1, 2, 0, 4, 0, 1, 2, 0, 0, 1,
                        0, 3, 0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 4, 0, 0, 4, 1, 3, 0, 0, 4, 0, 2, 0, 4, 0, 0, 0, 1, 0,
                        2, 4, 0, 1, 1, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 4, 0, 4, 3, 4, 3, 3, 0, 4, 2, 3, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 3, 2, 2, 0, 0, 4, 0, 0, 0, 0, 0, 0, 4, 0, 2, 3,
                        3, 0, 0, 0, 0, 0, 2, 0, 3, 4, 3, 0, 0, 1, 0, 0, 2, 1, 2, 2, 2, 0, 4, 2, 4, 0, 2, 1, 0, 1,
                        4, 1, 0, 3, 3, 2, 1, 4, 3, 0, 3, 0, 1, 0, 0, 1, 1, 3, 4, 0, 1, 0, 0, 0, 2, 0, 3, 0, 3, 2,
                        4, 3, 4, 2, 2, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 4, 0, 4, 3, 2, 3, 2, 0, 2, 0, 0,
                        0, 1, 3, 2, 0, 0, 0, 0, 0, 4, 0, 2, 0, 1, 0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 0, 1,
                        0, 0, 1, 0, 0, 1, 0, 0, 2, 1, 4, 2, 2, 1, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 2, 3, 4, 4, 0, 0,
                        0, 3, 4, 1, 0, 0, 3, 0, 0, 3, 0, 0, 1, 3, 0, 1, 3, 0, 0, 3, 0, 0, 0, 0, 2, 0, 3, 0, 1, 0,
                        0, 1, 4, 1, 4, 0, 0, 0, 3, 0, 4, 0, 2, 4, 0, 0, 0, 0, 0, 2, 0, 0, 2, 0, 0, 0, 1, 0, 3, 4,
                        0, 0, 0, 0, 0, 2, 3, 0, 2, 0, 3, 4, 2, 0, 4, 0, 0, 3, 2, 0, 2, 2, 2, 3, 0, 1, 4, 4, 0, 2,
                        2, 4, 3, 0, 2, 4, 2, 0, 1, 4, 4, 0, 2, 1, 0, 0, 0, 0, 0, 2, 0, 1, 3, 3, 0, 0, 2, 0, 2, 3,
                        0, 3, 0, 1, 0, 0, 0, 3, 2, 0, 3, 0, 0, 0, 4, 3, 3, 0, 0, 2, 1, 0, 0, 2, 2, 0, 0, 0, 3, 4,
                        0, 4, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 4, 0, 3, 0, 0, 0, 2, 2, 3, 0, 0, 1, 4, 1, 2, 0, 2, 4,
                        0, 4, 0, 0, 1, 2, 1, 4, 4, 0, 0, 0, 2, 0, 2, 0, 0, 0, 0, 3, 3, 2, 1, 0, 0, 0, 4, 0, 2, 0,
                        0, 0, 0, 0, 0, 2, 4, 0, 0, 4, 4, 0, 4, 2, 3, 0, 2, 2, 0, 0, 0, 2, 4, 0, 0, 2, 4, 3, 3, 0,
                        2, 4, 0, 0, 0, 1, 4, 1, 1, 0, 2, 0, 0, 0, 2, 0, 3, 0, 0, 1, 0, 0, 1, 0, 2, 0, 0, 0, 0, 2,
                        0, 0, 2, 0, 0, 3, 0, 0, 2, 0, 3, 4, 2, 3, 0, 0, 4, 3, 1, 4, 2, 0, 2, 4, 4, 0, 0, 1, 0, 4,
                        2, 4, 1, 4, 0, 0, 4, 4, 0, 0, 0, 0, 1, 0, 2, 3, 4, 0, 0, 4, 0, 0, 0, 0, 3, 0, 1, 0, 0, 3,
                        0, 2, 0, 0, 0, 0, 0, 1, 0, 1, 0, 2, 0, 3, 0, 0, 0, 1, 3, 0, 2, 3, 2, 2, 3, 0, 0, 0, 0, 3,
                        0, 0, 3, 0, 1, 0, 0, 0, 1, 0, 0, 3, 1, 2, 0, 1, 0, 0, 4, 2, 3, 4, 4, 0, 0, 2, 4, 3, 3, 0};
    int ro[N], sumRo[N], k[N], maxK;
    ofstream fout("itog.txt");
    vector<int> t;
    //t.reserve(N);
    for(int i=0; i<N;i++)
        t.push_back(0);

    vector<int> maxV, Vvariant;
    bool flag;
    int iSize,jSize = 6;
    iSize = N/jSize;

    //последовательный этап
    for(i=0;i<N;i++){
        ro[i] = 0;
        sumRo[i] = 0;
        k[i] = 0;
    }

    /* for(i=0;i<iSize;i++){
        if(i==0)
            t[0]=0;
        else
            t[i*jSize]=t[(i-1)*jSize]-i;
        for(j=1;j<jSize;j++)
            if(j<iSize)
                t[i*jSize + j] = t[i*jSize + j -1]-j;
            else
                t[i*jSize + j] = t[i*jSize + j -1]-iSize;
    }*/

    for(i=0;i<iSize;i++)        //that's linear+diagonal priorities with *10 for better discretisation
        for(j=0;j<jSize;j++)
            if(i==j)
                t[i*jSize+j]=-ceil(10*i*sqrt(2));
            else
                if(i<j)
                    t[i*jSize+j]=10*(-j+i)-ceil(10*i*sqrt(2));
                else
                    t[i*jSize+j]=10*(-i+j)-ceil(10*j*sqrt(2));

    /*
    for(i=0;i<iSize;i++){
        if(i==0)
            t.push_back(0);
        else
            t[i*iSi ze]=t[(i-1)*iSize]-i;
        for(j=1;j<jSize;j++)
            if(j<iSize)
                t[i*iSize + j] = t[i*iSize + j -1]-j;
            else
                t[i*iSize + j] = t[i*iSize + j -1]-iSize;
    }*/
    fout << "Priority matrix:" << endl;
    for(i=0;i<iSize;i++){
        for(j=0;j<jSize;j++)
            fout<<setw(3)<<t[i*jSize+j]<<" ";
        fout<<endl;
    }
    fout << endl;
    fout << "Ro matrix:" << endl;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++)
            ro[i] += matrix[i*N + j];
        k[i] = -ro[i];
        fout <<"V["<<setw(2)<<i<<"] = "<< ro[i] << endl;
    }
    k[0] = USED;
    for(i=0;i<N;i++)
        if(k[i] != USED)
            k[i] += 2*matrix[i*N+0];


    flag = true;
    while(flag){
        maxK = USED;
        fout << "K matrix:" << endl;
        for(i=0;i<N;i++){
            if(k[i] != USED){
                if(k[i] > maxK){
                    maxK = k[i];
                    maxV.clear();
                    maxV.push_back(i);
                }
                else if(k[i] == maxK)
                    maxV.push_back(i);
            }
            fout <<"V["<<setw(2)<<i<<"] = " << k[i] << endl;
        }
        if(maxK == USED) flag = false;
        for(auto i:maxV){
            for(j=0;j<N;j++)
                if(k[j] != USED)
                    k[j] += 2*matrix[j*N + i];
            k[i] = USED;
        }
        t=pushV(maxV, t,iSize);
        fout <<"MAX K = "<< maxK << endl;
        for(i=0;i<iSize;i++){
            for(j=0;j<jSize;j++)
                fout << setw(3)<< t[i*jSize + j] << " ";
            fout << endl;
        }
        maxV.clear();
        //cout << maxK << endl;
        //getchar();
    }
    //swap(t[5],t[6]);
    fout << "finish posled" << endl;
    for(i=0;i<iSize;i++){
        for(j=0;j<jSize;j++)
            fout << setw(3)<<t[i*jSize + j] << " ";
        fout << endl;
    }
    Q = findQ(matrix, t, jSize);
    Qlast = Q;
    fout << "Q = " << Q << endl;
    fout << "iteration" << endl;
    //итерационный
    //Q = findQ(matrix, t, jSize);
    //Qlast = Q;
    cout << "Q = " << Q << endl;
    flag = true;
    while(flag){
        flag = false;
        Vbad = findVbad(matrix, t, jSize, ro, fout);
        cout << "Vbad = "<<Vbad<<endl;
        fout << "Vbad = "<<Vbad<<endl;
        Xc = findCenter(matrix, t, jSize, ro[Vbad],Vbad,'%');
        cout << "delta Xc = " << Xc <<endl;
        fout << "Xc = " << Xc <<endl;
        //Xc += (distance(t.begin(),find(t.begin(),t.end(),Vbad)))%jSize;
        //cout <<"Xc real = " << Xc << endl;
        Yc = findCenter(matrix, t, jSize, ro[Vbad],Vbad,'/');
        cout << "delta Yc = " << Yc <<endl;
        fout << "Yc = " << Yc <<endl;
        //Yc += (distance(t.begin(),find(t.begin(),t.end(),Vbad)))/jSize;
        //cout <<"Yc real = " << Yc << endl;
        //if(Xc < 0.0) Xc = 0.0;
        //if(Xc > (jSize-1)) Xc = jSize -1;
        //if(Yc < 0.0) Yc = 0.0;
        //if(Yc > (iSize-1)) Yc = jSize -1;

        cout << "Xc = " << Xc << " Yc = " << Yc << endl;
        getchar();
        Vvariant.clear();
        if(ceil(Xc) == floor(Xc))
            if(ceil(Yc) == floor(Yc))
                Vvariant.push_back(Yc*jSize+Xc);
            else{
                if(ceil(Yc) < iSize)    Vvariant.push_back(ceil(Yc)*jSize+Xc);
                if(floor(Yc) >= 0)Vvariant.push_back(floor(Yc)*jSize+Xc);
            }
        else
            if(ceil(Yc) == floor(Yc)){
                if(ceil(Xc) < jSize) Vvariant.push_back(Yc*jSize+ceil(Xc));
                if(floor(Xc) >= 0) Vvariant.push_back(Yc*jSize+floor(Xc));
            }
            else{
                if(ceil(Yc) < iSize){
                    if(ceil(Xc) < jSize)
                        Vvariant.push_back(ceil(Yc)*jSize+ceil(Xc));
                    if(floor(Xc) >= 0)
                        Vvariant.push_back(ceil(Yc)*jSize+floor(Xc));
                }
                if(floor(Yc) >= 0){
                    if(ceil(Xc) < jSize)
                        Vvariant.push_back(floor(Yc)*jSize+ceil(Xc));
                    if(floor(Xc) >= 0)
                        Vvariant.push_back(floor(Yc)*jSize+floor(Xc));
                }
            }
        int indexVbad = distance(t.cbegin(),find(t.cbegin(),t.cend(),Vbad));
        int indexVchange;
        fout << "Vchange   Q" << endl;
        for(auto v:Vvariant){
            cout << "Vchange: " << t[v] << endl;
            //fout << "V[" << t[v] <<"]"<< endl;
        }
        for(auto v:Vvariant){
            swap(t[v],t[indexVbad]);
            Q = findQ(matrix, t, jSize);
            if(Q < Qlast){
                Qlast = Q;
                indexVchange = v;
                flag = true;
            }
            swap(t[v],t[indexVbad]);
            fout << "V[" << t[v] <<"] = " << Q << endl;
        }
        if(flag)
            swap(t[indexVchange],t[indexVbad]);
        cout << "Q = " << Qlast << endl << endl;
        for(i=0;i<iSize;i++){
            for(j=0;j<jSize;j++){
                cout << t[i*jSize + j] << " ";
                fout <<setw(2) << t[i*jSize + j] << " ";
            }
            cout << endl;
            fout << endl;
        }
        getchar();
    }
    return 0;
}
