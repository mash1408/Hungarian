# include <iostream>
# include <vector>
# include <vector>
# include <iterator>
# include <sstream>
# include <set>
# include <algorithm>

using namespace std;
int vectorSize=0;
int workers=0;
vector<vector <int>> optimalIndices {};

void standardForm(vector<vector<int>> &matrix){
   int max=matrix.front().size();
   for(auto item: matrix)
    if(item.size()>max)
        max= item.size();
vectorSize=max;
    for(auto &item: matrix)
        if(item.size()<max)
            for (int i=item.size(); i<max;i++)
                item.push_back(-1);
}

void modifyRow(vector<int> &row){
    int min= row.front();
    vector<int>::iterator it;
    for (it = row.begin(); it != row.end(); ++it)
        if(*it<min && *it!=-1)
            min=*it;
    for (it = row.begin(); it != row.end(); ++it)
    if(*it!=-1)
        *it=*it-min;
}
void modifyColumn(vector<vector<int>> &matrix){
    int min;
    for(int j=0;j<vectorSize;j++){
        min=100;
        for(int i=0;i<workers;i++)
            if(matrix.at(i).at(j)<min && matrix.at(i).at(j)!=-1)
                min=matrix.at(i).at(j);
         for(int k=0;k<workers;k++)
            matrix.at(k).at(j)-=min;
    }
       
}
bool assigned(int row, int col){
   for(auto item: optimalIndices)
        if(item.at(0) == row && item.at(1)== col)
            return true;
    return false;
}
int checkOneZeroRow(vector<int> row,int rowNum){
    
    if(count(row.begin(), row.end(), 0) >1 || count(row.begin(), row.end(), 0) ==0 )
        return -1;
    else{
        auto it = find(row.begin(), row.end(), 0);
       
    // If element was found
    if (it != row.end())
    {
        int index = it - row.begin();
         if(assigned(rowNum,index))
            return -3;
        return index;
    }
    } 
}
int checkOneZeroCol(vector<vector <int>> matrix, int colNum){
        int count=0;int index=0;
        for(int i=0;i<workers;i++){
            if(matrix.at(i).at(colNum)==0){
                count++;
                index=i;
            }
        }
        if(count>1 || count == 0)
            return -1;
        else if(assigned(index,colNum))
            return -3;
        else 
            return index; 
}
void crossOutZeroCol(vector<vector <int>> &matrix, int colNum,int row){
     for(int i=0;i<workers;i++){
         if(matrix.at(i).at(colNum) == 0 && i!=row)
            matrix.at(i).at(colNum)=-2;
     }
}
void crossOutZeroRow(vector<vector <int>> &matrix, int colNum,int row){
     for(int i=0;i<vectorSize;i++){
         if(matrix.at(row).at(i) == 0 && i!=colNum)
            matrix.at(row).at(i)=-2;
     }
}
void print(vector<vector<int>> costMatrix){
    for(auto row: costMatrix){
for (vector<int>::iterator it = row.begin(); it != row.end(); ++it)
    cout<<*it<<"\t";
cout<<endl;
}
}

bool optimal(){
    if(optimalIndices.size() != workers || optimalIndices.size() ==0)
        return false;
    else 
        return true; 
}

int optimalValue(vector<vector<int>> costMatrix){
    int sum=0;
    for(auto item: optimalIndices)
        sum+=costMatrix.at(item.at(0)).at(item.at(1));
    return sum;
}
void compute(vector<vector<int>> &costMatrix){
     
    for(int i=0;i<2;i++){
        int j=0;
    for(auto row: costMatrix){
        int index=checkOneZeroRow(row,j);
        if(index!=-1 && index!=-3){
            optimalIndices.push_back({j,index});
            crossOutZeroCol(costMatrix,index,j);
        }
        j++;
    }
    }
     for(int i=0;i<2;i++){
         for(int j=0;j<vectorSize;j++){
             int index= checkOneZeroCol(costMatrix,j);
             
            if(index!=-1 && index!=-3){
                optimalIndices.push_back({index,j});
                crossOutZeroRow(costMatrix,j,index);
            }
         }
     }
       
}
void sanitise(vector<vector<int>> &costMatrix){
             for(auto &row: costMatrix)
                for(auto &item: row)
                    if(item == -2)
                        item=0;
            
}
int main(){
vector<vector<int>> costMatrix;

cout<<"Enter the number of workers"<<endl;
cin>>workers;
for(int i=0;i<workers;i++){
    cout<<"Enter cost of job" << i+1<<endl;
    cin>>ws;
    string line;
    getline(cin,line);
    int value;
    istringstream input(line);
vector<int> result;
    while (input >> value)
    {
        result.push_back(value);
    }
    costMatrix.push_back(result);
}
standardForm(costMatrix);

vector<vector<int>> copyMatrix (costMatrix);

for(auto &item: costMatrix)
    modifyRow(item);
     
modifyColumn(costMatrix); 
print(costMatrix);

int iter=0;
while(!optimal()){
    optimalIndices.clear();
    cout<<"************************************Output after iteration "<< iter<<"**************************************"<<endl;
    compute(costMatrix);
    cout<<endl;
    print(costMatrix);
    set <int> checkedRows;
    set <int> checkedCols;
    int i=0;
    // if(iter == 2)
    //     break;
    //check rows
    for(auto row: costMatrix){
        if(!assigned(i,find(row.begin(), row.end(), 0)-row.begin()))
            checkedRows.insert(i);
        i++;
    }
    //check columns
    for(int i=0;i<vectorSize;i++){
        for(auto checkedRowNum: checkedRows)
            if(costMatrix.at(checkedRowNum).at(i) == 0 || costMatrix.at(checkedRowNum).at(i) ==-2){ 
                 checkedCols.insert(i);
                 break;
            }              
    }
  
    // check rows not already checked having assignments in checked columns
    for(auto checkCol: checkedCols)
        for(auto item: optimalIndices)
            if(item.at(1)==checkCol)
                checkedRows.insert(item.at(0));
    
//    for (set<int> :: iterator  itr = checkedRows.begin(); itr != checkedRows.end(); itr++) {
//         cout << *itr << " ";
//     }
//     cout<<endl;
//     for (set<int> :: iterator  itr = checkedCols.begin(); itr != checkedCols.end(); itr++) {
//         cout << *itr << " ";
//     }
    //finding unchecked rows
    set <int> unCheckedRows;
    for(int i=0;i<workers;i++)
        if (checkedRows.find(i) == checkedRows.end())
            unCheckedRows.insert(i);
//    for (set<int> :: iterator  itr = unCheckedRows.begin(); itr != unCheckedRows.end(); itr++) {
//         cout << *itr << " ";
//     }    
    int min=100;
    for(int i=0;i<workers;i++){
        for(int j=0;j<vectorSize;j++){
            if(costMatrix.at(i).at(j)<min && unCheckedRows.find(i) == unCheckedRows.end() && checkedCols.find(j) == checkedCols.end() && costMatrix.at(i).at(j)!=-1)
                min=costMatrix.at(i).at(j);
        }
    }

    for(int i=0;i<workers;i++)
        for(int j=0;j<vectorSize;j++){
            if(unCheckedRows.find(i) == unCheckedRows.end() && checkedCols.find(j) == checkedCols.end())
                if(costMatrix.at(i).at(j)!=0 && costMatrix.at(i).at(j) != -2 )
                costMatrix.at(i).at(j)-=min;
                else 
                    costMatrix.at(i).at(j)-=min+2;
            else if(unCheckedRows.find(i)!= unCheckedRows.end() && checkedCols.find(j) != checkedCols.end() )
                if(costMatrix.at(i).at(j)!=0 && costMatrix.at(i).at(j) != -2 )
                    costMatrix.at(i).at(j)+=min;
                else 
                    costMatrix.at(i).at(j)+=min+2;

        }
    iter++;
  
    sanitise(costMatrix);
}


cout<<endl<<"Optimal value: "<<optimalValue(copyMatrix)<<endl;
}