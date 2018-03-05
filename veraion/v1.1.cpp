#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int turn = 0;               //صفر يعني لم يختر ملف واحد تم اختيار ملف
string fileName = "back";   //اسم الملف back = لم يختر ملف
ifstream file;              //الملف
void printScreen();         //دالة الطباعة
void chooseFile();          //دالة اختيار ملف
void Detect();              //دالة كشف الاخطاء

int main()
{
    char ch;
    do{
        system("cls");
        printScreen();
        if(turn) cout<<"Selected File is (\'"<<fileName<<"\')\n"<<endl;
        cout<<"Enter Number : ";
        cin>>ch;
        switch(ch)
        {
            case '1': chooseFile();
                break;
            case '2':{
                if(turn)
                {
                    Detect();
                    system("PAUSE");
                }//EndIF
                else{
                    cout<<"\nThe File does not Exist Please Choose your File\n\n";
                    system("PAUSE");
                }//EndElse
                break;
            }//EndCase = 2
            case '3': if(turn){ file.close(); } return 0;
                break;
        }
    }while(ch != '3');
    return 0;
}//End main


void printScreen()
{
    cout << "\n\t\t\tName           : Abdulmalik Ben Ali"<<"\n\t\t\tStudent Number : 213010551" << endl;
    cout <<"\n\t\t1- Select The File to Detect\n\t\t   (\'The File you Must be Next to The Program\')."<<endl;
    cout <<"\n\t\t2- Edit code."<<endl;
    cout <<"\n\t\t3- Exit.\n\n\n\n"<<endl;
}//End printScreen

void chooseFile()
{
    int Count = 0;
    if(turn)
    {
        file.close();
        turn = 0;
    }
    do{
        system("cls");
        cout<<"Existing Files : "<<endl;
        system("dir /b /a-d *.txt");
        ( Count == 0 )?  cout<<endl<<"Enter name file (*.txt) or back : " :
                        cout<<endl<<"The File(\'"<<fileName<<"\') does not Exist \nPlease Enter name file (*.txt) or back : ";
        cin>>fileName;
        Count++;
        if(fileName == "back") return;
        file.open(fileName);
    }while(!file);
    turn = 1;
}//End chooseFile

void Detect()
{
    if(!turn) return;
    char c;
    file.clear();
    file.seekg(0,ios::beg);
    vector<char> temp;
    int Token(0),   //Count token
        Line(1),    //Count line
        col(0),     //Count column
        scol(0),    //start token column
        logic(1),   //0 or 1
        Error(0);   //Count Error

    system("cls");
    cout<<"Selected File is (\'"<<fileName<<"\')\n"<<endl;
    while(file.get(c))
    {
        logic = 1;
        col++;
        temp.clear();
        if(c ==' ') continue;  //back while 'skip space' #1
        else if(c =='\n') //line end in file
        {
            Line++;
            col = 0;
            continue;
        }//end if new line
        else if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))//#2
        {
            Token++;
            cout<<"\tToken # "<<Token<<" = \'";
            scol = col;
            do{
                cout<<c;
                file.get(c);
                col++;
            }while(  ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ) && !file.eof() );// //end Do while
            col--;
            file.unget();        //#3
            cout<<"\' at Line "<<Line<<" ,col "<<scol<<endl;
            continue;
        }//end if a-z
        else if(c >= '0' && c <= '9')       //#4
        {
            Token++;
            cout<<"\tToken # "<<Token<<" = \'";
            scol = col;
            do{
                cout<<c;
                file.get(c);
                col++;
            }while( (c >= '0' && c <= '9') && !file.eof() );// //end Do while

            col--;
            file.unget();         //#5
            cout<<"\' at Line "<<Line<<" ,col "<<scol<<endl;
            continue;
        }//end if digit

        else if(c == '{')     //#6
        {
            scol = col;
            do{
                temp.push_back(c);
                file.get(c);
                col++;
                if(c == '\n')//#6.5
                {
                    cout<<endl<<string(60,'*')<<endl;
                    cout<<"***Error Comment is not Complete***\n\'";
                    for(int i = 0; i < int(temp.size()); i++)
                    {
                        cout << temp[i];
                    }
                    cout<<"\' at Line "<<Line<<endl;
                    cout<<string(60,'*')<<endl<<endl;
                    logic = 0;
                }
            }while( c != '}' && logic && !file.eof() );//end Do while
            if(logic) continue;     // #7 end comment
        }//end if { comment }

        else if(c == '(')//#8
        {
            Token++;
            file.get(c);
            if(c == '*')//#9
            {
                Token--;
                temp.push_back('(');
                do{
                    temp.push_back(c);
                    file.get(c);
                    col++;
                    if(c == '\n' || file.eof())//#6.5
                    {
                        x:
                        cout<<endl<<string(60,'*')<<endl;
                        cout<<"***Error Comment is not Complete***\n\'";
                        for(int i = 0; i < int(temp.size()); i++)
                        {
                            cout << temp[i];
                        }
                        cout<<"\' at Line "<<Line<<endl;
                        cout<<string(60,'*')<<endl<<endl;
                        logic = 0;
                    }
                    else if(c == '*')//#10
                    {
                        temp.push_back(c);
                        file.get(c);
                        col++;
                        if( c == '\n' ){ col = 0; Line++; goto x; }
                        if(c == ')')//#11
                        {
                            logic = 0;

                        }//end if )
                        else{
                            file.unget();
                            col--;
                        }
                    }//end if c==*
                }while( logic && !file.eof() );// //end Do while
            }//end if c==*
            else
            {
                file.unget();  //20
                cout<<"\tToken # "<<Token<<" = \'(\' at Line "<<Line<<" ,col "<<col<<endl;
                continue;
            }
        }//end if ( or (* comment *)

        else if(c == ')')
        {
            Token++;
            cout<<"\tToken # "<<Token<<" = \')\' at Line "<<Line<<" ,col "<<col<<endl;
            continue;
        }//end if )

        else if( c == ':' ) //#12
        {
            Token++;
            file.get(c);
            col++;
            if( c == '=' ) //#13
            {
                cout<<"\tToken # "<<Token<<" = \':=\' at Line "<<Line<<" ,col "<<col<<endl;
            }else{
                col--;
                file.unget();  //#20
                cout<<"\tToken # "<<Token<<" = \':\' at Line "<<Line<<" ,col "<<col<<endl;
            }

        }//end if c==:

        else if( c == '<' )//#14
        {
            Token++;
            file.get(c);
            col++;
            if( c == '=' )//#15
            {
                cout<<"\tToken # "<<Token<<" = \'<=\' at Line "<<Line<<" ,col "<<col<<endl;
            }else if( c == '>'){    //#16
                cout<<"\tToken # "<<Token<<" = \'<>\' at Line "<<Line<<" ,col "<<col<<endl;
            }else{
                col--;
                file.unget();     //#20
                cout<<"\tToken # "<<Token<<" = \'<\' at Line "<<Line<<" ,col "<<col<<endl;
            }

        }//end if c==<

         else if( c == '>' ) //#12
        {
            Token++;
            file.get(c);
            col++;
            if( c == '=' ) //#13
            {
                cout<<"\tToken # "<<Token<<" = \'>=\' at Line "<<Line<<" ,col "<<col<<endl;
            }else{
                col--;
                file.unget();  //#20
                cout<<"\tToken # "<<Token<<" = \'>\' at Line "<<Line<<" ,col "<<col<<endl;
            }

        }//end if c==>
         else if(c == '\"')     //#string
        {
            Token++;
            scol = col;
            do{
                temp.push_back(c);
                file.get(c);
                col++;
                if(c == '\n')//#
                {
                    cout<<endl<<string(60,'*')<<endl;
                    cout<<"***Error string is not Complete***\n\'";
                    for(int i = 0; i < int(temp.size()); i++)
                    {
                        cout << temp[i];
                    }
                    cout<<"\' at Line "<<Line<<endl;
                    cout<<string(60,'*')<<endl<<endl;
                    logic = 0;
                }
                else if( c == '\"')
                {
                    cout<<"\tToken # "<<Token<<" = \'";
                    for(int i = 0; i < int(temp.size()); i++)
                    {
                        cout << temp[i];
                    }
                    cout<<"\' at Line "<<Line<<" ,col "<<col<<endl;
                }
            }while( c != '"' && logic && !file.eof() );//end Do while
            if(logic) continue;     // #7 end comment
        }//end if " string "
        else {
            Error++;
            cout<<endl<<string(60,'*')<<endl;
            cout<<"Error # "<<Error<<", **Error at line "<<Line<<",Col "<<col<<", Unknown Character** = "<<c<<endl;
            cout<<string(60,'*')<<endl<<endl;
        }
    }// file != end of file
}//End Detect
