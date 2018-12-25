#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <fstream>
#include <locale>

using namespace std;

class Node
{
public:
    int entries;
    char symbol;
    Node *left, *right;

    Node () //перегрузка конструктора
    {left=right=NULL;}

    Node(Node *L, Node *R)  //конструктор, принимает 2 ссылки (на сыновей)
                              //создает отца
    {
        left = L;
        right = R;
        entries = L->entries + R->entries;
    }
};

struct Compare
{
    bool operator() (const Node* l, const Node* r) const
    { return l->entries < r->entries; }
};

void print(Node *root, unsigned k = 0)
{
    if (root != NULL)
    {
        print(root->left, k+3);

        for (unsigned i = 0; i<k; i++)
            cout << "   ";

        /*if (root->c)
            cout << root->a << " (" << root->c << ")" << endl;*/
        if (root->left == NULL && root->right == NULL)
            cout << root->entries << " (" << root->symbol << ")" << endl;
        else
            cout << root->entries << endl;
        print(root->right, k+3);
    }
}

vector<bool> code;
map<char, vector<bool> > table;  //ассоциация символа с кодом

void BuildTable (Node *root)
{
    if (root->left!=NULL)
    {
        code.push_back(0);   //если слева не 0, ставим в вектор 0
        BuildTable(root->left);
    }

    if (root->right!=NULL)   //если справа не 0, ставим в вектор 1
    {
        code.push_back(1);
        BuildTable(root->right);
    }

    if (root->left == NULL && root->right == NULL) //если нашли букву
        table[root->symbol] = code;  //ассоциируем эту букву с кодом

    code.pop_back();  //сокращаем код на 1 цифру
}

int main()
{    
    unsigned r;
    cout << "Выберите способ ввода строки:" << endl <<
            "1. Из файла" << endl << "2. С консоли"<<endl;
    cin >> r;
    string s;
    cin.ignore();

    map<char, int> count_of_entry; //для подсчета количества вхождений символов

    if (r==1)
    {
        ifstream f("input.txt");
        if(!f.is_open())
        {
            cerr << "Ошибка. Файл не найден.";
            exit(1);
        }
        else
            getline(f, s);
        f.close();
    }
    if (r==2)
    {
        cout << endl << "Введите строку: ";
        getline(cin, s);
    }

    for (int i = 0; i < s.length(); i++) //считаем вхождения
    {
        char c = s[i];
        count_of_entry[c]++;     //m от символа увеличиваем на 1
    }

    cout << "Количество вхождений символов в строку:" << endl;

    map<char, int>::iterator iter;
    for (iter=count_of_entry.begin(); iter!=count_of_entry.end(); iter++)
        cout << iter->first << ": " << iter->second << endl;

    //"засеиваем" список этими вершинами с кол-вом вхождений

    list<Node*> list;   //список указателей на узлы

    for (iter=count_of_entry.begin(); iter!=count_of_entry.end(); ++iter)
    {
        Node *p = new Node;  //создаем новый узел и инициализируем его
        p->symbol = iter->first;
        p->entries = iter->second;
        list.push_back(p);       //указатель на узел скидываем в лист ( в списке хранятся указатели)
    }

    while (list.size()!=1) //на каждом шаге убирается 2 элемента и добавляется 1
    {                   //останется 1 переменная - вершина дерева
        list.sort(Compare());  //сортировка списка по кол-ву вхождений
        //SonL, SonR - левый, правый сын, вспомогательные переменные
        Node *SonL = list.front(); //присваиваем переменную
        list.pop_front();          //удаляем её из списка
        Node *SonR = list.front();
        list.pop_front();

        Node *parent = new Node(SonL, SonR); //создается отец двух сыновей
        list.push_back(parent);                 //кладется в список
    }

    Node *root = list.front(); //достали корень дерева

    cout << "Вид дерева Хаффмана: " << endl;
    print(root);

    BuildTable(root);//создадим таблицу значений (символ-его код)

    cout << "Код Хаффмана: " << endl;

    for (int i=0; i<s.length(); i++)
    {
        char c=s[i];  //вспомогательная переменная с
        vector<bool> x =  table[c]; //всп. перем. х, содержит код символа с

        for (int n=0; n < x.size(); n++)  //выводим код символа
            cout << x[n];
    }

    cout << endl;
    return 0;
}

