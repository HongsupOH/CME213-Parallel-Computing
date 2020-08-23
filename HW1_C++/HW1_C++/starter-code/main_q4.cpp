#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <numeric>
#include <stdexcept>


/**********  Q4a: DAXPY **********/

template <typename T>
std::vector<T> daxpy(T a, const std::vector<T>& x, const std::vector<T>& y)
{
    // TODO
    std::vector<T> res;
    res.resize(x.size());
    //std::for_each(x.begin(),x.end());
    std::transform(x.begin(),x.end(),res.begin(),std::bind1st(std::multiplies<T>(),a));
    std::transform(res.begin(),res.end(),y.begin(),res.begin(),std::plus<T>());
    return res;
}

/**********  Q4b: All students passed **********/
constexpr double HOMEWORK_WEIGHT = 0.20;
constexpr double MIDTERM_WEIGHT = 0.35;
constexpr double FINAL_EXAM_WEIGHT = 0.45;

struct Student
{
    double homework;
    double midterm;
    double final_exam;

    Student(double hw, double mt, double fe) : 
           homework(hw), midterm(mt), final_exam(fe) { }
};

bool all_students_passed(const std::vector<Student>& students, double pass_threshold) 
{
    // TODO
    int cnt = 0; 
    for(int i=0;i<students.size();i++){
        Student st = students[i];
        double score = st.homework*HOMEWORK_WEIGHT+st.midterm*MIDTERM_WEIGHT+st.final_exam*FINAL_EXAM_WEIGHT;
        if (score>=pass_threshold){
            cnt++;
        }
    }
    if (cnt==students.size()){
        return true;
    }
    return false;
}


/**********  Q4c: Odd first, even last **********/
bool sort_help(int left, int right)
{
    if (left & 0 && right &0){
        return left<right;
    }
    else if (left & 0){
        return false;
    }
    else if (right & 0){
        return true;
    }
    return left<right;
    // TODO
}

void sort_odd_even(std::vector<int>& data)
{
    // TODO
    std::sort(data.begin(),data.end(),sort_help);

}

/**********  Q4d: Sparse matrix list sorting **********/
struct SparseMatrixCoordinate
{

    int row;
    int col;
    int data;
    
    SparseMatrixCoordinate(int r, int c, int d) :
        row(r), col(c), data(d) { }
};

struct Node
{

    int row;
    int col;
    int value;
    Node *next; 
};

void addNode(struct Node** start, int ele, int rowInd, int colInd){
    struct Node *temp, *r, *p;
    temp = *start;
    if (temp==NULL){
        temp = (struct Node *) malloc (sizeof(struct Node));
        temp->value = ele;
        temp->row = rowInd;
        temp->col = colInd;
        temp->next = NULL;
        *start = temp;
    }
    else{
        while(temp->next!=NULL){
            temp=temp->next;
        }
        r = (struct Node *)malloc(sizeof(struct Node));
        r->value = ele;
        r->row = rowInd;
        r->col = colInd;
        r->next = NULL;
        temp->next = r;
    }
}

void sortedInsert(struct Node** head_ref, struct Node* new_node){
    struct Node* current;
    if (*head_ref == NULL || (*head_ref)->row>new_node->row || ((*head_ref)->row==new_node->row && (*head_ref)->col>new_node->col)){
        new_node->next = *head_ref;
        *head_ref = new_node;
    }
    else{
        current = *head_ref;
        while(current->next!=NULL && 
        (current->next->row < new_node->row || (current->next->row == new_node->row) && 
        (current->next->col < new_node->col))){
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
}

void insertionSort(struct Node **head_ref){
    struct Node *sorted  = NULL;
    struct Node *current = *head_ref;
    while (current != NULL){
        struct Node *next = current->next;
        sortedInsert(&sorted,current);
        current = next;
    }
    *head_ref = sorted;
}

void sparse_matrix_sort(std::list<SparseMatrixCoordinate>& list) 
{
    // TODO
    struct Node* start = NULL;
    std::list<SparseMatrixCoordinate>::iterator it;
    for (it = list.begin();it != list.end(); ++it){
        int val = it->data;
        int row = it->row;
        int col = it->col;
        addNode(&start, val,row,col);
    }
    insertionSort(&start);
    while (start != NULL){
        std::cout<<"row: "<<start->row<<","<<"col: "<<start->col<<","<<"value: "<<start->value<<std::endl;
        start = start->next;
    }
    
}


int main() 
{    
    // Q4a test
    const int Q4_A = 2;
    const std::vector<int> q4a_x = {-2, -1, 0, 1, 2};
    const std::vector<int> q4_y = {-2, -1, 0, 1, 2};
    std::vector<int> res;
    res = daxpy(Q4_A,q4a_x,q4_y);
    std::cout<<"Q4-a Result: "<<std::endl;
    std::cout<<"[";
    for (int i=0;i<res.size()-1;i++){
        std::cout<<res[i]<<",";
    }
    std::cout<<res[res.size()-1];
    std::cout<<"]"<<std::endl;
    // TODO: Verify your Q4a implementation

    // Q4b test
    std::vector<Student> all_pass_students = {
            Student(1., 1., 1.),
            Student(0.6, 0.6, 0.6),
            Student(0.8, 0.65, 0.7)};

    bool ex1 = all_students_passed(all_pass_students,0.6);
    std::cout<<"Q4-b Result: "<<std::endl;
    if(ex1==1){
        std::cout<<"All Pass"<<std::endl;
    }
    else{
        std::cout<<"Not all Pass"<<std::endl;
    }

    std::vector<Student> not_all_pass_students = {
            Student(1., 1., 1.),
            Student(0, 0, 0)};

    bool ex2 = all_students_passed(not_all_pass_students,60);
    if(ex2==1){
        std::cout<<"All Pass"<<std::endl;
    }
    else{
        std::cout<<"Not all Pass"<<std::endl;
    }

    // TODO: Verify your Q4b implementation

    // Q4c test
    std::vector<int> odd_even_sorted = {-5, -3, -1, 1, 3, -4, -2, 0, 2, 4};

    // TODO: Verify your Q4c implementation
    std::cout<<"Q4-c Result: "<<std::endl;
    //std::sort(odd_even_sorted.begin(),odd_even_sorted.end(),sort_odd_even);
    sort_odd_even(odd_even_sorted);
    std::cout<<"[";
    for (int i=0;i<odd_even_sorted.size()-1;i++){
        std::cout<<odd_even_sorted[i]<<",";
    }
    std::cout<<odd_even_sorted[odd_even_sorted.size()-1];
    std::cout<<"]"<<std::endl;

    // Q4d test
    std::list<SparseMatrixCoordinate> sparse = {
            SparseMatrixCoordinate(2, 5, 1),
            SparseMatrixCoordinate(2, 2, 2),
            SparseMatrixCoordinate(4, 4, 5),
            SparseMatrixCoordinate(3, 4, 3),
            SparseMatrixCoordinate(0, 0, 4)};

    // TODO: Verify your Q4d implementation
    sparse_matrix_sort(sparse);

    //sparse_matrix_sort(sparse);
    return 0;
}
