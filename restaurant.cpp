#include "main.h"
int MAXSIZE;
template<typename E> class HuffNode{
protected:
    int height;
    int heightofNode(HuffNode<char>*node){
        if(!node) return 0;
        if(node->isLeaf()) return 1;
        return 1+max(heightofNode(node->left()), heightofNode(node->right()));
    }
public:
    virtual int weight()=0;
    virtual E val(){ return '\0';}
    virtual bool isLeaf() = 0;
    virtual HuffNode<E>* left()=0;
    virtual HuffNode<E>* right()=0;
    virtual int getHeight()=0;
    virtual void setLeft(HuffNode<E>*b){};
    virtual void setRight(HuffNode<E>*b){};
    virtual ~HuffNode(){};
    void updateHeight(){
        this->height= heightofNode(this);
    }

};
// Base destructor
// Return frequency
// Determine type
template <typename E>   // Leaf node subclass
class LeafNode : public HuffNode<E> {
private:
    E it; // Value
    int wgt;              // Weight
public:
    LeafNode(const E& val, int freq)
    {
        it = val;
        wgt = freq;
        this->height=1;
    }
    int weight() override { return wgt; }
    E val() override{ return it; }
    bool isLeaf() override { return true; }
    HuffNode<E>* left() override{return nullptr;}
    HuffNode<E>* right() override{return nullptr;}
    int getHeight() override{return this->height;}


};
// Constructor
template <typename E>   // Internal node subclass
class IntlNode : public HuffNode<E> {
private:
    HuffNode<E>* lc;
    HuffNode<E>* rc;
    int wgt;
    // Left child
    // Right child
    // Subtree weight
public:
    IntlNode(HuffNode<E>* l, HuffNode<E>* r)
    {
        wgt = l->weight() + r->weight();
        lc = l; rc = r;
        this->height=1+max(r->getHeight(),l->getHeight());
    }
    int weight() override{ return wgt; }
    bool isLeaf() override{ return false; }
    HuffNode<E>* left() override{ return lc; }
    HuffNode<E>* right() override{ return rc; }
    void setLeft(HuffNode<E>* b) override
    { lc = (HuffNode<E>*)b; }
    void setRight(HuffNode<E>* b) override
    { rc = (HuffNode<E>*)b; }
    int getHeight() override{return this->height;}
};
template<typename E>
class HuffTree {
private:
    HuffNode<E>* Root;
public:
    HuffTree(E val, int freq)
    { Root = new LeafNode<E>(val, freq); }
    // Internal node constructor
    HuffTree(HuffTree<E>* l, HuffTree<E>* r)
    { Root = new IntlNode<E>(l->root(), r->root()); }
    int weight() { return Root->weight(); }
    E& val(){return '\0';}
    void setRoot(HuffNode<E>*cur){this->Root=cur;}
    HuffNode<E>* root() const;
    void removeTree(){
        if(!this->Root) return;
        queue<HuffNode<char>*>q;
        q.push(this->Root);
        while(!q.empty()){
            int n = q.size();
            for (int i = 0; i < n; ++i) {
                HuffNode<char>*cur=q.front();
                q.pop();
                if(cur->left()) q.push(cur->left());
                if(cur->right()) q.push(cur->right());
                cur->setLeft(nullptr);
                cur->setRight(nullptr);
                delete cur;
            }
        }
        this->Root= nullptr;
    }

    static int Balance(HuffNode<E>*cur){
        if(cur->isLeaf()) return 0;
        return cur->left()->getHeight()-cur->right()->getHeight();
    }
    static HuffNode<char>* rotateLeft(HuffNode<char>*cur){
        HuffNode<E>*x=cur;
        HuffNode<E>*y=cur->right();
        x->setRight(y->left());
        y->setLeft(x);
        x->updateHeight();
        y->updateHeight();
        return y;
    }
    static HuffNode<char>* rotateRight(HuffNode<char>*cur){
        HuffNode<E>*x=cur;
        HuffNode<E>*y=cur->left();
        x->setLeft(y->right());
        y->setRight(x);
        x->updateHeight();
        y->updateHeight();
        return y;
    }
};

template<typename E>
HuffNode<E> *HuffTree<E>::root() const { return Root; }

class Inserter
{
private:
    struct MyCompareHuff{
        bool operator()(const pair<HuffTree<char>*,int>& a,const pair<HuffTree<char>*,int>& b){
            if(a.first->weight()==b.first->weight())
                return a.second>b.second;
            return a.first->weight()>b.first->weight();
        }
    };
    struct Comparator
    {
        bool operator()(const pair<char,int>*a,const pair<char,int>*b){
            if(a->second==b->second){
               if(islower(a->first)&&isupper(b->first)) return true;
               if(isupper(a->first)&& islower(b->first)) return false;
               return a->first<b->first;
            }
            return a->second<b->second;
        }
    };
    HuffTree<char>* curTree;
    HuffNode<char>* BalanceNode(HuffNode<char>*root,int& count){
        if(!root) return nullptr;
        int balance = HuffTree<char>::Balance(root);
        if (balance > 1 && HuffTree<char>::Balance(root->left()) >= 0)
        {
            root = HuffTree<char>::rotateRight(root);
            count++;
        }
        else if (balance > 1 && HuffTree<char>::Balance(root->left()) < 0)
        {
            root->setLeft(HuffTree<char>::rotateLeft(root->left()));
            root = HuffTree<char>::rotateRight(root);
            count++;
        }
        else if (balance < -1 && HuffTree<char>::Balance(root->right()) <= 0)
        {
            root = HuffTree<char>::rotateLeft(root);
            count++;
        }
        else if (balance < -1 && HuffTree<char>::Balance(root->right()) > 0)
        {
            root->setRight(HuffTree<char>::rotateRight(root->right()));
            root = HuffTree<char>::rotateLeft(root);
            count++;
        }
        if(count<3&&root->left()) root->setLeft(BalanceNode(root->left(),count));
        if(count<3&&root->right()) root->setRight(BalanceNode(root->right(),count));
        root->updateHeight();
        return root;
    }
    list<pair<char,int>*>*  buldList(string& name)
    {
        map<char, int> freq;
        for (const char &chr : name)
        {
            if (freq.count(chr) == 0)
            {
                freq[chr] = 1;
            }
            else
            {
                freq[chr]++;
            }
        }
        if(freq.size()<3) return nullptr;
        string Caesername;
        Caesername = "";
        for(const char& ch:name){
            if(isupper(ch)){
                Caesername += char(ch+freq[ch]-'A')%26+'A';
            }
            else{
                Caesername += char(ch+freq[ch]-'a')%26+'a';
            }
        }
        name = Caesername;
        map<char, int> ans;
        for (auto & it : freq)
        {
            char ch = it.first;
            int num = it.second;
            char cur;
            if(isupper(ch)){
                cur = char(ch+num-'A')%26+'A';
            }
            else{
                cur = char(ch+num-'a')%26+'a';
            }
            if(ans.find(cur)!=ans.end()){
                ans[cur]+=num;
            }
            else{
                ans[cur]=num;
            }
        }
        auto listX = new list<pair<char,int>*>();
        for(auto & an : ans){
            char key = an.first;
            int val = an.second;
            auto newone = new pair<char,int>(key,val);
            listX->push_back(newone);
        }
        listX->sort(Comparator());
        return listX;
    }
    void buildTree(list<pair<char,int>*>*listX){
        int order = 0;
        priority_queue<pair<HuffTree<char>*,int>,vector<pair<HuffTree<char>*,int> >,MyCompareHuff>pq;
        while(!listX->empty()){
            auto cur = listX->front();
            listX->pop_front();
            auto newTree= new HuffTree<char>(cur->first,cur->second);
            pair<HuffTree<char>*,int>newNode(newTree,++order);
            pq.push(newNode);
            delete cur;
        }
        delete listX;

        HuffTree<char>*temp1,*temp2,*temp3;
        while (pq.size() > 1) {
                temp1 = pq.top().first;
                pq.pop();
                temp2 = pq.top().first;
                pq.pop();
                temp3 = new HuffTree<char>(temp1, temp2);
                int count = 0;
                temp3->setRoot(BalanceNode(temp3->root(), count));
                pair<HuffTree<char> *, int> newroot(temp3, ++order);
                pq.push(newroot);
                delete temp1;
                delete temp2;
            }
        if(this->curTree){
            this->curTree->removeTree();
            delete this->curTree;
        }
        this->curTree=pq.top().first;
    }
    void DFS_encoding(HuffNode<char>*root,string path,map<char,string>&ans){
        if(!root) return;
        if(root->isLeaf()){
            ans [root->val()]=path;
        }
        DFS_encoding(root->left(),path+"0",ans);
        DFS_encoding(root->right(),path+"1",ans);
    }
    string encoding(string name){
        map<char,string>encode_table;
        DFS_encoding(this->curTree->root(),"",encode_table);
        string ans;
        ans="";
        for(const char& ch:name){
            ans = ans + encode_table.at(ch);
        }
        return ans.length()<=10?ans:ans.substr(ans.length()-10,10);

    }
    int toDecimal(string bin){
        int k = 0;
        int ans=0;
        for(int i =bin.length()-1;i>=0;--i){
            ans+=(pow(2,k)*(bin[i]-'0'));
            k++;
        }
        return ans;
    }
    void inorderofTree(){
        if(!this->curTree->root()) return;
        stack<HuffNode<char>*>stk;
        HuffNode<char>*cur = this->curTree->root();
        while(cur||!stk.empty()){
            while (cur) {
                stk.push(cur);
                cur = cur->left();
            }
            cur = stk.top();
            stk.pop();

            if(cur->isLeaf()){
                cout<<cur->val()<<"\n";
            }
            else{
                cout<<cur->weight()<<"\n";
            }
            cur = cur->right();
        }
    }
public:
    Inserter(){
        this->curTree= nullptr;
    }
    ~Inserter(){
        delete this->curTree;
    }
    void hand(){
        if(!this->curTree) return;
        inorderofTree();
    }
    int load(string name){
        list<pair<char,int>*>*listX=buldList(name);
        if(!listX) return -1;
        buildTree(listX);
        string bin = encoding(name);
        reverse(bin.begin(),bin.end());
        int result = toDecimal(bin);
        return result;
    }
};
class Restaurant
{
public:

public:
    virtual void LAPSE(string name) = 0;
    virtual void KEITEIKEN(int num) = 0;
    virtual void KOKUSEN() = 0;
    virtual void HAND() = 0;
    virtual void LIMITLESS(int num) = 0;
    virtual void CLEAVE(int num) = 0;
};
class S_res{
private:
    class Zone{
    private:
        vector<int>*customers;
        int label;
        friend class S_res;
    public:
        Zone(int label){
            this->label=label;
            this->customers=new vector<int>();
        }
        ~Zone(){
            delete customers;
        }
        int showLabel(){return label;}
        void insert(int newcus){
            this->customers->push_back(newcus);
        }
        void remove(int k){
                for(int i =0;i<k&&this->customers->size()>0;++i){
                    cout<<this->customers->front()<<"-"<<label<<"\n";
                    this->customers->erase(this->customers->begin());
                }
        }
        void LIFOprint(int num){
            int i =0;
            if(this->customers->size()==0) return;
            for (auto it = customers->rbegin(); it != customers->rend()&&i<num; ++it){
                cout<<label<<"-"<<*it<<endl;
                ++i;
            }
        }
        int size(){return this->customers->size();}
    };
    class S_heap{
    private:
        Zone** zones;
        Zone** data;
        int count;
        list<int>*order;
        bool compareZone(Zone*a,Zone*b){
            if(a->size()==b->size()){
                int i,j,k;
                k = 0;
                i=j=-1;
                for(int & it : *order){
                    if(a->showLabel()==it) i=k;
                    if(b->showLabel()==it) j=k;
                    if(i!=-1&&j!=-1) break;
                    k++;
                }
                return i>j;
            }
            return a->size()<b->size();
        }
        void reheapDown(int pos){
            int k = pos;
            int left=pos*2+1;
            int right=pos*2+2;
            if(left<count) {
                k = compareZone(data[k], data[left]) ? k : left;
            }
            if(right<count) {
                k = compareZone(data[k], data[right]) ? k : right;
            }
            if(k!=pos){
                swap(data[k],data[pos]);
                reheapDown(k);

            }
        }
        void reheapUp(int pos){
            while (pos>0)
            {
                int parent = (pos-1)/2;
                if(compareZone(data[parent],data[pos])) break;
                swap(data[pos],data[parent]);
                pos=parent;
            }
        }
        void Change(int zone)
        {
            auto it = order->begin();
            while (it != this->order->end() && *it != zone)
            {
                ++it;
            }
            this->order->erase(it);
            this->order->push_front(zone);
        }
        void sort(vector<int>&arr){
            int n = arr.size();
            for(int i = 1;i<n;++i){
                int temp = arr[i];
                int j = i-1;
                while(j>=0&&compareZone(zones[temp-1],zones[arr[j]-1])){
                    arr[j+1]=arr[j];
                    --j;
                }
                arr[j+1]=temp;
            }
        }
        void insertNode(Zone*newzone){
            this->data[count++]=newzone;
            reheapUp(count-1);
        }
        void deleteNode(int pos){
            if(pos==count-1){
                this->count--;
                return;
            }
            swap(this->data[pos],this->data[count-1]);
            this->count--;
            if(this->count<=1) return;
            if(pos<=(this->count-2)/2){
                this->reheapDown(pos);
            }
            else{
                this->reheapUp(pos);
            }
        }
    public:
        S_heap(){
            this->zones=new Zone*[MAXSIZE];
            this->data=new Zone*[MAXSIZE];
            this->order=new list<int>();
            this->count=0;
            for(int i =1;i<=MAXSIZE;++i){
                this->order->push_back(i);
                this->zones[i-1]=new Zone(i);
            }
        }
        ~S_heap(){
            for(int i =0;i<MAXSIZE;++i){
                delete this->zones[i];
            }
            delete this->data;
            delete this->zones;
            delete this->order;
        }
        void insertZone(int label,int result){
            this->Change(label);
            this->zones[label-1]->insert(result);
            if (this->zones[label - 1]->size() == 1)
            {
                this->insertNode(this->zones[label - 1]);
            }
            else{
                for(int i =0;i<count;++i){
                    if(data[i]->showLabel()==label){
                        reheapDown(i);
                    }
                }
            }

        }
        void deleteZone(int label,int k)
        {
            this->Change(label);
            for(int i = 0; i<this->count;++i){
                if(label==this->data[i]->showLabel()){
                    this->data[i]->remove(k);
                    if(this->data[i]->size()==0)
                        this->deleteNode(i);
                    else
                        this->reheapUp(i);
                    return;

                }
            }
        }
        vector<int> Kthlargest(int k){
            vector<int>cur;
            for(int i=0;i<this->count;++i){
                cur.push_back(this->data[i]->showLabel());
            }
            sort(cur);
            if(k>=count) return cur;
            vector<int>ans;
            for(int i=0;i<k;++i){
                ans.push_back(cur[i]);
            }
            return ans;
        }
        void PreorderPrint(int num){
            if(this->count==0) return;
            stack<int>s;
            s.push(0);
            while(!s.empty()){
                int cur = s.top();
                s.pop();
                data[cur]->LIFOprint(num);
                int left = cur*2+1;
                int right = cur*2+2;
                if(right<count) s.push(right);
                if(left<count) s.push(left);
            }
        }
        int size(){return this->count;}


    };
    S_heap* min_heap;
public:
    S_res(){
        this->min_heap=new S_heap();
    }
    void insert(int result){
        int id=result%MAXSIZE+1;
        this->min_heap->insertZone(id,result);
    }
    void keiketen(int num){
        vector<int>cur=this->min_heap->Kthlargest(num);
        for(const int&ele:cur){
            this->min_heap->deleteZone(ele,num);
        }
    }
    void cleave(int num){
        this->min_heap->PreorderPrint(num);
    }
};
class G_res{
private:
    class BST{
    private:
        struct Node{
            int customer;
            Node* left;
            Node* right;
            Node(int data):customer(data),left(nullptr),right(nullptr){}
        };
        int label;
        Node* root;
        queue<int>* order;
        Node* insertRec(Node* node,int newcus){
            if(!node) {
                return new Node(newcus);
            }
            else if(newcus<node->customer)
                node->left=insertRec(node->left,newcus);
            else
                node->right=insertRec(node->right,newcus);
            return node;
        }
        Node* deleteRec(Node*node,int key){
            if(!node) return nullptr;
            if(node->customer<key){
                node->right= deleteRec(node->right,key);
                return node;
            }
            else if(node->customer>key){
                node->left= deleteRec(node->left,key);
                return node;
            }
            else{
                if(!node->left){
                    Node* temp=node->right;
                    delete node;
                    return temp;
                }
                else if(!node->right){
                    Node* temp=node->left;
                    delete node;
                    return temp;
                }
                else{
                    Node* cur = node->right;
                    while(cur->left){
                        cur = cur->left;
                    }
                    node->customer=cur->customer;
                    node->right= deleteRec(node->right,cur->customer);
                    return node;
                }
            }
        }
        void postOrderRec(Node*node,vector<int>&ans){
            if(!node) return;
            postOrderRec(node->left,ans);
            postOrderRec(node->right,ans);
            ans.push_back(node->customer);
        }
        void removeTree(){
            if(!this->root) return;
            queue<Node*>q;
            q.push(root);
            while (!q.empty()){
                int n = q.size();
                for(int i =0;i<n;++i){
                    Node* cur =q.front();
                    q.pop();
                    if(cur->left) q.push(cur->left);
                    if(cur->right) q.push(cur->right);
                    cur->left=cur->right= nullptr;
                    delete cur;
                }
            }
        }
    public:
        BST(int label){
            this->root =  nullptr;
            this->label = label;
            this->order = new queue<int>();

        }
        ~BST(){
            this->removeTree();
            this->root= nullptr;
        }
        void insert(int newcus){
            this->order->push(newcus);
            this->root=insertRec(root,newcus);
        }
        void remove(int k){
            for(int i = 0;i<k&&!order->empty();++i){
                int cur = order->front();
                order->pop();
                this->root = deleteRec(this->root,cur);
            }
        }
        vector<int> postOrder(){
            vector<int>ans;
            postOrderRec(this->root,ans);
            return ans;
        }
        void printTree(){
            if(!this->root) return;
            stack<Node*>s;
            Node*curr = this->root;
            while (curr != NULL || s.empty() == false) {
                while (curr != NULL) {
                    s.push(curr);
                    curr = curr->left;
                }
                curr = s.top();
                s.pop();
                cout << curr->customer <<"\n";
                curr = curr->right;
            }
        }
    };
    int count;
    BST** zones;
    vector<vector<int>> PascalTriagle(int n){
        vector<vector<int>>ans;
        vector<int>first;
        first.push_back(1);
        ans.push_back(first);
        for(int i =1;i<n;++i){
            vector<int>cur;
            cur.push_back(1);
            for(int j=0;j<i-1;++j){
                cur.push_back(ans.back()[j]+ans.back()[j+1]);
            }
            cur.push_back(1);
            ans.push_back(cur);
        }
        return ans;
    }
    long long sameTree(vector<int>&postorder,vector<vector<int>>&Pascal){
        if(postorder.size()<=1) return 1;
        vector<int>left;
        vector<int>right;
        int n = postorder.size();
        for(int i = 1;i< n;++i){
            if(postorder[i]<postorder[0]){
                left.push_back(postorder[i]);
            }
            else{
                right.push_back(postorder[i]);
            }
        }
        unsigned int a=left.size();
        unsigned int b=right.size();
        long long leftside = sameTree(left,Pascal);
        long long rightside = sameTree(right,Pascal);
        long long ans = (Pascal[a + b][a] * leftside) * rightside;
        return ans;
    }
    int permutationofPost(vector<int>postorder){
        reverse(postorder.begin(), postorder.end());
        vector<vector<int>>Pascal= PascalTriagle(postorder.size()+1);
        long long ans = sameTree(postorder,Pascal);
        int res = ans%MAXSIZE;
        return res;
    }
public:
    G_res(){
        this->count=0;
        this->zones=new BST*[MAXSIZE];
        for(int i=0;i<MAXSIZE;++i){
            this->zones[i]=new BST(i+1);
        }
    }
    ~G_res(){
        for(int i=0;i<MAXSIZE;i++){
            delete zones[i];
        }
        delete zones;
    }
    void insert(int result){
        this->count++;
        int S_zone = result%MAXSIZE+1;
        this->zones[S_zone-1]->insert(result);
    }
    void kokusen(){
        for(int i=0;i<MAXSIZE;++i){
            BST* cur = this->zones[i];
            vector<int> postOrder = cur->postOrder();
            int k=permutationofPost(postOrder);
            cur->remove(k);
        }
    }
    void limitless(int num){
        BST* cur = zones[num-1];
        cur->printTree();
    }
};

class imp_res : public Restaurant
{
private:
    Inserter* inserter;
    G_res* gojo_res;
    S_res* sakuma_res;
public:
    imp_res(){
        this->inserter=new Inserter();
        this->gojo_res=new G_res();
        this->sakuma_res=new S_res();
    }
    ~imp_res(){
        delete inserter;
        delete gojo_res;
        delete sakuma_res;
    }
    void LAPSE(string name) override{
        int result=this->inserter->load(name);
        if(result==-1) return;
        if(result%2!=0){
            this->gojo_res->insert(result);
        }
        else{
            this->sakuma_res->insert(result);
        }
    }
    void KOKUSEN() override{
        this->gojo_res->kokusen();
    }
    void KEITEIKEN(int num) override{
        this->sakuma_res->keiketen(num);
    }
    void HAND() override{
        this->inserter->hand();
    }
    void LIMITLESS(int num) override{
        this->gojo_res->limitless(num);
    }
    void CLEAVE(int num) override{
        this->sakuma_res->cleave(num);
    }
};

void simulate(string filename) {
    int i=2;
    ifstream ss;
    ss.open(filename);

    string str, maxsize, name, num;
    ss>>str;
    ss>>maxsize;
    MAXSIZE= stoi(maxsize);
    Restaurant* r = new imp_res();
    while (ss >> str) {
        if(str=="LAPSE"){
//            cout<<"LAPSE AT LINE "+ to_string(i)<<endl;
            ss>>name;
//            if(name=="WYBisPhicwxsWNsyatWixTvtPeuQaqkYYgghi"){
//                int k=1;
//            }
            r->LAPSE(name);
        }
        else if(str=="KOKUSEN"){
//            cout<<"KOKUSEN AT LINE "+to_string(i)<<endl;
            r->KOKUSEN();
        }
        else if(str=="KEITEIKEN"){
//            cout<<"KEITENKEN AT LINE "+ to_string(i)<<endl;
            ss>>num;
            r->KEITEIKEN(stoi(num));
        }
        else if(str=="HAND"){
//            cout<<"HAND AT LINE "+ to_string(i)<<endl;
            r->HAND();
        }
        else if(str=="LIMITLESS"){
//            cout<<"LIMITLESS AT LINE "+ to_string(i)<<endl;
            ss>>num;
            r->LIMITLESS(stoi(num));
        }
        else if(str=="CLEAVE"){
//            cout<<"CLEAVE AT LINE "<<to_string(i)<<endl;
            ss>>num;
            r->CLEAVE(stoi(num));
        }
        else{
            cout<<"ERROR";
        }
        i++;
    }
    ss.close();
}