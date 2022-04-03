#include <bits/stdc++.h>
using namespace std;
class bucket{
    public:
    int local_depth,occupancy,bucket_capacity;
    //vector<int> v;
    int *arr;
    bucket(int LD,int bcap);
};
bucket::bucket(int LD,int bcap){
    local_depth=LD;
    occupancy=0;
    bucket_capacity=bcap;
    arr=new int[bcap];
    //v.reserve(bcap);
}
class hash_table{
    int bucketcapacity,globaldepth,globaldepthlimit;
    vector<bucket*> directory,bucket_list;
    public:
    hash_table(int GD,int BC);
    bool insert_el(int n);
    bool delete_el(int n);
    bool search_el(int n);
    void display();
};
hash_table::hash_table(int GD,int BC){
    bucketcapacity=BC;
    globaldepth=GD;
    globaldepthlimit=20;
    int N=1LL<<GD;
    for(int i=0;i<N;i++){
        bucket *bucket_ptr=new bucket(GD,BC);
        directory.push_back(bucket_ptr);
        bucket_list.push_back(bucket_ptr);
    }
}
bool hash_table::insert_el(int n){
    //If No duplicates allowed
        if(search_el(n)){
            cout<<n<<" already exists in hashtable"<<endl;
            return false;
        } 
    int bitmask=(1LL<<globaldepth)-1;
    int hofn=(bitmask&n);
    //Introduce hash function
        //hofn=n%7;
    bucket* bucketptr=directory[hofn];
    int size=bucketptr->occupancy;
    //C-1:Empty place found
    if(size<bucketcapacity){
        bucketptr->arr[size]=n;
        bucketptr->occupancy=size+1;
        return true;
    }
    //Recursive solution
        //C-2:No empty place but localdepth<global depth(make new bucket + redistribute)
        if(bucketptr->local_depth<globaldepth){
            //Fixing pointers
            bucket* newbucketptr=new bucket(bucketptr->local_depth,bucketcapacity);
            bucket_list.push_back(newbucketptr);
            int isone=(1LL<<((bucketptr->local_depth)));
            int currbitmask=(1LL<<bucketptr->local_depth)-1;
            int reqdbits=currbitmask&hofn;
            for(int i=0;i<directory.size();i++){
                if((i&currbitmask)==reqdbits){
                    if((isone&i)!=0){
                        directory[i]=newbucketptr;
                    }
                }
            }
            bucketptr->local_depth=bucketptr->local_depth+1;
            newbucketptr->local_depth=newbucketptr->local_depth+1;
            //Redistribute
            int i=0,j=0;
            for(int k=0;k<bucketcapacity;k++){
                int curr=bucketptr->arr[k];
                if((curr&isone)!=0){
                    newbucketptr->arr[j]=curr;
                    newbucketptr->occupancy=newbucketptr->occupancy+1;
                    bucketptr->occupancy=bucketptr->occupancy-1;
                    j++;
                }
                else{
                    bucketptr->arr[i]=curr;
                    i++;
                }
            }
            return insert_el(n);
        }
        if(globaldepth==globaldepthlimit){
            cout<<"Value cannot be accomodated"<<endl;
            return false;
        }
        //C-3:No empty place and localdepth=global depth(double the directory + make new bucket + redistribute)
        int currsize=directory.size();
        for(int i=0;i<currsize;i++){
            directory.push_back(directory[i]);
        }
        globaldepth=globaldepth+1;
        return insert_el(n);
}
bool hash_table::search_el(int n){
    int bitmask=(1LL<<globaldepth)-1;
    int hofn=(bitmask&n);
    //Introduce hash function
        //hofn=n%7;
    bucket* bucketptr=directory[hofn];
    int size=bucketptr->occupancy;
    for(int i=0;i<size;i++){
        if(bucketptr->arr[i]==n)return true;
    }
    return false;
}
bool hash_table::delete_el(int n){
    //Lazy method
    int bitmask=(1LL<<globaldepth)-1;
    int hofn=(bitmask&n);
    //Introduce hash function
        //hofn=n%7;
    bucket* bucketptr=directory[hofn];
    int size=bucketptr->occupancy;
    bool ans=false;
    //Deleting single occurance of a number
    for(int i=0;i<size;i++){
        if(bucketptr->arr[i]!=n)continue;
        int t=bucketptr->arr[size-1];
        bucketptr->arr[i]=t;
        bucketptr->occupancy=size-1;
        ans=true;
        break;
    }
    //Non lazy additional code
        // if(bucketptr->occupancy==0){
        //     //merge
        //     int y=n&((1<<(bucketptr->local_depth))-1);
        //     int x=1<<((bucketptr->local_depth)-1);
        //     int reqdbucket=y^x;
        //     bucket* reqdbucketptr=directory[reqdbucket];
        //    if(bucketptr->local_depth!=1 && globaldepth!=1 && reqdbucketptr!=bucketptr){
        //         for(int i=0;i<directory.size();i++){
        //             if(directory[i]==bucketptr){
        //                 directory[i]=reqdbucketptr;
        //             }
        //         }
        //         delete bucketptr;
        //         reqdbucketptr->local_depth=reqdbucketptr->local_depth-1;
        //         //deleting from bucket_list
        //         for(int i=0;i<bucket_list.size();i++){
        //             if(bucket_list[i]==bucketptr){
        //                 while(i+1<bucket_list.size()){
        //                     bucket_list[i]=bucket_list[i+1];
        //                     i++;
        //                 }
        //                 bucket_list.pop_back();
        //             }
        //         }
        //     }
        // }
        // //halving directory if reqd
        // bool check=false;
        // for(int i=0;i<directory.size();i++){
        //     if(directory[i]->local_depth==globaldepth)
        //         check=true;
        // }
        // if(!check){
        //     int initsize=directory.size();
        //     for(int i=0;i<initsize/2;i++){
        //         directory.pop_back();
        //     }
        //     globaldepth--;
        // }
    return ans;
    //Deleting all occurances of a number
    // bool t=false;
    // for(int i=0;i<size;){
    //     if(bucketptr->arr[i]!=n){
    //         i++;
    //         continue;
    //     }
    //     int t=bucketptr->arr[size-1];
    //     bucketptr->arr[i]=t;
    //     bucketptr->occupancy=size-1;
    //     size--;
    //     t=true;
    // }
    // return t;
}
void hash_table::display(){
    cout<<globaldepth<<endl;
    cout<<bucket_list.size()<<endl;
    for(auto bucketptr:bucket_list){
        cout<<(bucketptr->occupancy)<<" "<<(bucketptr->local_depth)<<endl;
        //Displaying keys
        // for(int i=0;i<bucketptr->occupancy;i++){
        //     cout<<bucketptr->arr[i]<<" ";
        // }
        // cout<<endl;
    }
}
int main(){
    int GD,BC;
    cin>>GD>>BC;
    hash_table h1(GD,BC);
    int t;
    cin>>t;
    while(t!=6){
        switch(t){
            case 2:{
                int n;
                cin>>n;
                bool t=h1.insert_el(n);
                break;
            }
            case 3:{
                int n;
                cin>>n;
                bool t=h1.search_el(n);
                if(t){
                    //cout<<n<<" exists in hashtable"<<endl;
                }else{
                    //cout<<n<<" does not exist in hashtable"<<endl;
                }
                break;
            }
            case 4:{
                int n;
                cin>>n;
                bool t=h1.delete_el(n);
                break;
            }
            case 5: h1.display();
                    break;
        }
        cin>>t;
    }
}