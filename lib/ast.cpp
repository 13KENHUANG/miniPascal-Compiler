
#include  "../include/ast.h"

using namespace std;


bool function_noreturn_show = true;
vector<string> Global_Show; //store global variable, function ID
void Show_data(MySymbolTable K_symbolTable,vector<string> ctr)
{
  string c1; //name 
  int    d1; //scope
  string c2; //type
  SymbolTable_Element element;
   for(int i=ctr.size()-1;i>=0;i--)
    {
        string key = ctr[i];
        element = K_symbolTable.findSymbol(key);
        c1 = element.name;
        d1 = element.scope;
        c2 = element.type;
        printf(SYMTAB_ENTRY_FMT, c1.c_str(), d1, c2.c_str());
    }

}

vector<TreeNode> TreeDefine::getChildren() {return childNode;}
void TreeDefine::addChild(TreeNode node) {childNode.push_back(node);}

TreeNode TreeDefine::getSibling() {return siblingNode;}
void TreeDefine::setSibling(TreeNode siblingNode) {this->siblingNode = siblingNode;}

int TreeDefine::getLineNumber() {return lineNumber;}
void TreeDefine::setLineNumber(int lineNumber) { this->lineNumber = lineNumber;}

int TreeDefine::getColNumber() {return columnNumber;}
void TreeDefine::setColNumber(int colNumber) {this->columnNumber=colNumber;}

int TreeDefine::getNodeType() {return nodeType;}
void TreeDefine::setNodeType(int nodeType) {this->nodeType = nodeType;}

Object TreeDefine::getAttribute() {return attribute;}
void TreeDefine::setAttribute(Object attribute) {this->attribute = attribute;}
void TreeDefine::setAttribute(int attribute,int flag ) {this->attribute._int = attribute; this->attribute.flag = flag;}
void TreeDefine::setAttribute(double attribute) {this->attribute._double = attribute; this->attribute.flag = 2;}
void TreeDefine::setAttribute(string attribute) {this->attribute._string = attribute; this->attribute.flag = 3;}

ExpressionType TreeDefine::getExpType() {return expressionType;}
void TreeDefine::setExpType(ExpressionType expressionType) {this->expressionType = expressionType;}

ExpressionType TreeDefine::getRunTimeType() {return runtimeType;}
void TreeDefine::setRunTimeType(ExpressionType runtimeType) {this->runtimeType = runtimeType;}

void TreeDefine::printTree(TreeNode root, int level)
{
    if(level == 0)
        if(root == NULL) return;
    for(int i = 0; i < level; ++i){
        cout<<"    ";
    }
    cout<<"|-";
    cout<<" "<<mapping[root->getNodeType()]<<"(no.1)";
    cout<<" "<<mapping[root->getExpType()]<<"(no.2)";
    switch(root->getAttribute().flag)
    {
        case 1: cout << " " << root->getAttribute()._int<<"(no.3-1)"; break;
        case 2: cout << " " << root->getAttribute()._double<<"(no.3-2)"; break;
        case 3: cout << " " << root->getAttribute()._string<<"(no.3-3)"; break;
        case 4: cout << " " << mapping[root->getAttribute()._int]<<"(no.3-4)"; break;
        default: cout << " "<< mapping[0]<<"(no.3)"; break;
    }
   // int row = root->getLineNumber();
   // int col = root->getColNumber();
   // cout<<", row="<<row<<" ,col="<<col;
    cout<<endl;

    if(root->getSibling() == NULL)
        bitSet.set(level);
    else
        bitSet.reset(level);
        
    int cnt = 0;
    for(auto i=0u;i<root->getChildren().size();i++)
        if(root->getChildren().at(i) != NULL) cnt++; //count Child number
    int cnt2 = 0;
    for(auto i=0u;i<root->getChildren().size();i++){
        if(root->getChildren().at(i) != NULL){
            cnt2++;
            for(int j = 0; j < level + 1; ++j){
            //    if(bitSet[j] == false)
           //         cout<<"|";
            //    else
            //        cout<<" ";
                cout<<"    ";
            }
            if(cnt2 == cnt)
                bitSet.set(level + 1);
            else
                bitSet.reset(level + 1);
           // cout<<"|-"<<mapping[root->getChildren().at(i)->getNodeType()]<<"(no.4)"<<endl;
          //  cout<<"check point_2"<<endl;
            cout<<"(I'm Child node,i="<<i<<"), my parent is "<<mapping[root->getNodeType()]<<",Attr="<<mapping[root->getAttribute()._int]<<endl;
            printTree(root->getChildren().at(i), level + 2);
        }
    }
    TreeNode t = root->getSibling();
    if(t != NULL)
    {
        if(t->getSibling() == NULL)
            bitSet.set(level);
        else
            bitSet.reset(level);
      //  cout<<"check point_3"<<endl;
        cout<<"<<I'm Sibling node"<<">>"<<", my friend is "<<mapping[root->getNodeType()]<<endl;
        printTree(t, level);
    }
}

void TreeDefine::theCreate(TreeNode root,FILE *Jfile)
{
    string tmp;
    bool   bHasLabel=false;
    if(root!=NULL)
    { 
        SHOW_NEWSCP();
        string sym = root->getAttribute()._string;
       // int row =root->getLineNumber();
       // int col =root->getColNumber();
        SymbolTable_Element element;
        element.name = sym;
        element.scope  = 0;
        element.type   = "void";
        element.K_Type = "Prog";
     //   cout<<sym<<", row="<<row<<", col="<<col<<endl;
        if(symbolTable_Q.addSymbol(element.name,element))
        {
           SHOW_NEWSYM(element.name);
           Global_Show.push_back(element.name); 
           tmp = ".class public ";
           tmp+=element.name; 
           tmp+="\n";
           fprintf(Jfile,"%s",tmp.c_str());
           tmp=".super java/lang/Object\n";
           fprintf(Jfile,"%s",tmp.c_str());

           
        }
       
    }
    
    for(auto i=0u;i<root->getChildren().size();i++)
    {
        if(root->getChildren().at(i) != NULL)
        {
            //VAR

            if(mapping[root->getChildren().at(i)->getNodeType()]=="VAR")
            {
                Sub_Var_decl(symbolTable_Q,root->getChildren().at(i),0,Global_Show);
                TreeNode tmp = root->getChildren().at(i);
                while(tmp->getSibling()!=NULL)
                {
                    if(mapping[tmp->getSibling()->getNodeType()]=="VAR")
                        Sub_Var_decl(symbolTable_Q,tmp->getSibling(),0,Global_Show);
                    tmp =tmp->getSibling();
                }
            }
           // cout<<"VAR DONE"<<endl;

            //PRODUCE&FUNCTION
            if(mapping[root->getChildren().at(i)->getNodeType()]=="PROCEDURE"
              ||mapping[root->getChildren().at(i)->getNodeType()]=="FUNCTION")
            {
                FunPro_decl(symbolTable_Q,root->getChildren().at(i),0);
                TreeNode tmp = root->getChildren().at(i);
                while(tmp->getSibling()!=NULL)
                {
                    if(mapping[tmp->getSibling()->getNodeType()]=="PROCEDURE"
                     ||mapping[tmp->getSibling()->getNodeType()]=="FUNCTION")
                        FunPro_decl(symbolTable_Q,tmp->getSibling(),0);
                    tmp =tmp->getSibling();
                }
            }

            

            if(mapping[root->getChildren().at(i)->getNodeType()]=="LABEL")
            {
                bHasLabel=true;
                if(bHasLabel)
                {
                    tmp="method public static main([Ljava/lang/String;)V\n";
                    fprintf(Jfile,"%s",tmp.c_str());
                }
                New_Label_Check(symbolTable_Q,root->getChildren().at(i));
                
                TreeNode tmp = root->getChildren().at(i);
                while(tmp->getSibling()!=NULL)
                {
                    
                    if(mapping[tmp->getSibling()->getNodeType()]=="LABEL"
                    &&tmp->getSibling()->getChildren().size()>=1)
                    {
                        New_Label_Check(symbolTable_Q,tmp->getSibling());
                    }
                    tmp=tmp->getSibling();
                }
            }
        }
    }
    SHOW_CLSSCP();
    SHOW_SYMTAB_HEAD();
    Show_data(symbolTable_Q,Global_Show);
    SHOW_SYMTAB_TAIL();

    //====Jfile=====//
    if(!bHasLabel)
    {
        tmp=".method public static main([Ljava/lang/String;)V\n";
        fprintf(Jfile,"%s",tmp.c_str());
        tmp=".limit locals 50\n";
        fprintf(Jfile,"%s",tmp.c_str());
        tmp=".limit stack 50\n";
        fprintf(Jfile,"%s",tmp.c_str());
    }
    tmp="    return\n";
    fprintf(Jfile,"%s",tmp.c_str());
    tmp=".end method\n";
    fprintf(Jfile,"%s",tmp.c_str());
    fclose(Jfile);
    
}

void TreeDefine::Find_Fun_jj(MySymbolTable &myTable,TreeNode root)
{
    string node_type=mapping[root->getNodeType()];
    if(node_type=="PROC_ID"||node_type=="ID")
    {
        string name=root->getAttribute()._string;
        int row =   root->getLineNumber();
        int col =   root->getColNumber();
        // bool ff=myTable.bFind(name,0,row,col);
        myTable.bFind(name,0,row,col);
    }
    for(auto i=0u;i<root->getChildren().size();i++)
    {
        if(root->getChildren().at(i)!=NULL)
        {
            Find_Fun_jj(myTable,root->getChildren().at(i));
            if(root->getChildren().at(i)->getSibling()!=NULL)
                Find_Fun_jj(myTable,root->getChildren().at(i)->getSibling());
        }
    }
}

void TreeDefine::New_Label_Check(MySymbolTable &myTable,TreeNode root,bool suc_defunc,int ROW,int COL,int IF_index)
{
    //====for test====//
    /*string t_node_type = mapping[root->getNodeType()];
    int    t_row       = root->getLineNumber();
    int    t_col       = root->getColNumber();
    cout<<"node_type="<<t_node_type<<" ,row="<<t_row<<" ,col="<<t_col<<endl;
    cout<<"root child number="<<root->getChildren().size()<<endl;
    if(root->getChildren().at(0)!=NULL)
        cout<<"root child number nodetype="<<mapping[root->getChildren().at(0)->getNodeType()]<<endl;
    else 
        cout<<"I'm wrong"<<endl;*/
    //================//
    //cout<<"test:"<<mapping[root->getNodeType()]<<endl;
   // cout<<"IF_index="<<IF_index<<endl;
    bool check=true;
    int row_L,row_R ;
    int col_L,col_R ;
    string name;
    bool bHas ;
    TreeNode ch1;
    vector<string> left_type;
    vector<string> right_type;
    SymbolTable_Element element;
    if(root->getChildren().at(IF_index)!=NULL)
        ch1 = root->getChildren().at(IF_index);
    else 
        return;
  
    //確認未宣告的函數
    if(mapping[root->getNodeType()]=="LABEL" && mapping[ch1->getNodeType()]=="PROC_ID"&&root->getChildren().at(0)!=NULL)
    {
        //cout<<"name="<<ch1->getAttribute()._string<<endl;
        row_L = ch1->getLineNumber();
        col_L = ch1->getColNumber();
        bHas= myTable.bFind(ch1->getAttribute()._string,1,row_L,col_L);
        if(!bHas) 
        {
              check=false;
              //確認函數參數有沒有在symbol Table裡面
              for(auto i=0u;i<ch1->getChildren().size();i++)
              {
                  if(ch1->getChildren().at(i)!=NULL)
                  {
                       Find_Fun_jj(myTable,ch1->getChildren().at(i));
                  }
              }
        }
        else
        {
            vector<string> check_para;
            name = ch1->getAttribute()._string;
            element = myTable.findSymbol(name);
            left_type = Spilt_type(element.type);
            check_para.push_back(left_type[0]);
            for(auto i=0u;i<ch1->getChildren().size();i++)
            {
                if(ch1->getChildren().at(i)!=NULL)
                {
                    TreeNode ch3=ch1->getChildren().at(i);
                    if(mapping[ch3->getNodeType()]=="CONST"&&mapping[ch3->getExpType()]=="INT")
                        check_para.push_back("int");
                     else if(mapping[ch3->getNodeType()]=="CONST"&&mapping[ch3->getExpType()]=="REAL")
                        check_para.push_back("real");
                     else if(mapping[ch3->getNodeType()]=="PROC_ID"&&myTable.bFind(ch3->getAttribute()._string,0,ch3->getLineNumber(),ch3->getColNumber()))
                        check_para.push_back(myTable.findSymbol(ch3->getAttribute()._string).type);
                     else if(mapping[ch3->getNodeType()]=="OP")
                     {
                       for(auto i=0u;i<ch3->getChildren().size();i++)
                       {
                           if(ch3->getChildren().at(i)!=NULL)
                           {
                             TreeNode ch4=ch3->getChildren().at(i);
                             if(mapping[ch4->getNodeType()]=="CONST"
                              &&mapping[ch4->getExpType()]=="INT")
                                check_para.push_back("int");
                             else if(mapping[ch4->getNodeType()]=="CONST"
                                   &&mapping[ch4->getExpType()]=="REAL")
                                check_para.push_back("real");
                             else if(mapping[ch4->getNodeType()]=="PROC_ID"&&myTable.bFind(ch4->getAttribute()._string,0,ch4->getLineNumber(),ch4->getColNumber()))
                                check_para.push_back(myTable.findSymbol(ch4->getAttribute()._string).type);
                           }
                       }
                     }
                     //後續其他的參數
                     while(ch3->getSibling()!=NULL)
                     {
                        ch3=ch3->getSibling();
                        if(mapping[ch3->getNodeType()]=="CONST"&&mapping[ch3->getExpType()]=="INT")
                           check_para.push_back("int");
                        else if(mapping[ch3->getNodeType()]=="CONST"&&mapping[ch3->getExpType()]=="REAL")
                           check_para.push_back("real");
                        else if(mapping[ch3->getNodeType()]=="PROC_ID"&&myTable.bFind(ch3->getAttribute()._string,0,ch3->getLineNumber(),ch3->getColNumber()))
                           check_para.push_back(myTable.findSymbol(ch3->getAttribute()._string).type);
                     }

                }
            }

            if(check_para!=left_type)
            {
                fprintf(stderr,WRONG_ARGS,row_L,col_L,name.c_str());
                return ;
            }
        }
    }
    
    //檢查一般ID assign
    else if(mapping[ch1->getNodeType()]=="ASSIGN"&&mapping[ch1->getAttribute()._int]=="ID")
    {
      // cout<<"ASSIGN HH"<<endl;

       //檢查ASSIGN左值
       if(ch1->getChildren().at(0)!=NULL)
       {
           row_L = ch1->getChildren().at(0)->getLineNumber();
           col_L = ch1->getChildren().at(0)->getColNumber();
          
          if(myTable.bFind(ch1->getChildren().at(0)->getAttribute()._string,0,row_L,col_L))
          {
            element = myTable.findSymbol(ch1->getChildren().at(0)->getAttribute()._string);
            left_type.push_back(Spilt_type(element.type)[0]);
           // left_type = Spilt_type(element.type);
           // cout<<"left name="<<element.name<<endl;
           // cout<<"suc_defunc="<<suc_defunc<<endl;
        //    cout<<"elelment Ktype="<<element.K_Type<<endl;
        //    cout<<mapping[ch1->getChildren().at(0)->getNodeType()]<<endl;
            vector<string> kkp = Spilt_type(element.type);
            if(!suc_defunc&&kkp.size()>1)
             {
               fprintf(stderr,ASSIG_TYPE,row_L,col_L);
             }
          }
          else 
          {
           check=false;
          }
       }
       //檢查ASSIGN右值
       if(ch1->getChildren().at(1)!=NULL)
       {
           TreeNode ch2 = ch1->getChildren().at(1); //CONST,PROC_ID,PROC_FUNC,OP
           row_R = ch2->getLineNumber();
           col_R = ch2->getColNumber();
           string name = ch2->getAttribute()._string;
           string tmp_type = mapping[ch2->getNodeType()];
           //若右值為常數
           if(tmp_type=="CONST")
           {    
              // cout<<"test CONST"<<endl;
               if(mapping[ch2->getExpType()]=="INT")
                 right_type.push_back("int");
               else if(mapping[ch2->getExpType()]=="REAL")
                 right_type.push_back("real");
              if(right_type!=left_type&&check&&suc_defunc)
                 fprintf(stderr,ASSIG_TYPE,row_L,col_L);
           }
           //若右值為變數
           else if(tmp_type=="PROC_ID")
           {
               if(myTable.bFind(ch2->getAttribute()._string,0,row_R,col_R))
               {
                   element = myTable.findSymbol(ch2->getAttribute()._string);
                  // cout<<"I'm the="<<element.name<<endl;
                   vector<string> tmp = Spilt_type(element.type);
                   if(tmp.size()>1)
                        right_type.push_back(tmp[0]);
                   else
                        right_type = tmp;
                   //right_type = Spilt_type(element.type);
                   if(right_type!=left_type&&check&&suc_defunc)
                    fprintf(stderr,ASSIG_TYPE,row_L,col_L);
               }     
           }
           //若右值為函數
           else if(tmp_type=="PROC_FUNC")
           {
            //    cout<<"The Proc_func"<<endl;
               
               //確認函數名是否在symbol table中

               if(myTable.bFind(ch2->getAttribute()._string,1,row_R,col_R))
               {
                   vector<string> check_para;
                   element = myTable.findSymbol(ch2->getAttribute()._string);
                   right_type = Spilt_type(element.type);//取出該函數名存在symbol table中的type資料
                   Find_Fun_Para(myTable,ch2,check_para,right_type);
          
                   if(check_para!=right_type&&check)
                   {
                       fprintf(stderr,WRONG_ARGS,row_R,col_R,name.c_str());
                       return;
                   }
                   else if(right_type[0]!=left_type[0]&&check&&suc_defunc)
                      fprintf(stderr,ASSIG_TYPE,row_L,col_L);
               }
               else
               {
                   //若函數不在symbol table內,檢查其參數有無在symbol table內
                   if(ch2->getChildren().size()>0)
                   {
                       Check_fake_func_para(myTable,ch2->getChildren().at(0));
                   }
                   
               }  
           }
           //若右值為OP
           else if(tmp_type=="OP")
           {
           // cout<<"OP left name="<<element.name<<endl;
            //確認OP內有無 未宣告的變數
             New_OP(myTable,ch2,check);
             if(check)
             {
                vector<OP_element> my_OP;
                vector<string> soldier;
                Check_OP_type(myTable,ch2,my_OP,soldier);
              /* cout<<"=====the OP====="<<endl;
                for(int i=0;i<my_OP.size();i++)
                    cout<<my_OP[i].op_type<<" ";
                cout<<endl;
                cout<<"====the type===="<<endl;
                for(int i=0;i<soldier.size();i++)
                    cout<<soldier[i]<<" ";
                cout<<endl;*/
                for(int i=soldier.size()-1;i>=1;i--)
                {
                    if(soldier[i]==soldier[i-1])
                        my_OP.pop_back();
                    else
                    {
                        int index=my_OP.size()-1;
                        fprintf(stderr,ARITH_TYPE,my_OP[index].row,my_OP[index].col,my_OP[index].op_type.c_str());
                        return ;
                    } 
                }
                right_type.push_back(soldier[0]);
                vector<string> tmp_right = Spilt_type(soldier[0]);
                if(tmp_right.size()>1)
                {
                    if(tmp_right[0]!=left_type[0]&&suc_defunc)
                         fprintf(stderr,ASSIG_TYPE,row_L,col_L);
                }
                else{
                     if(right_type!=left_type&&suc_defunc)
                         fprintf(stderr,ASSIG_TYPE,row_L,col_L);
                }
             }
           }

       }
       
    }
    

    //檢查 Array ID assign
    else if(mapping[ch1->getNodeType()]=="ASSIGN"&&mapping[ch1->getAttribute()._int]=="ARRAY")
    {
       //cout<<"ASSIGN ARRAY"<<endl;
      //檢查ASSIGN index有無無宣告的變數

      Check_Array_Index(myTable,ch1,check);
      //cout<<"check="<<check<<endl;
      if(check)
      {
        //ch1->getChildren().at(0)為陣列開頭變數
         string name = ch1->getChildren().at(0)->getAttribute()._string;
         //cout<<"name="<<name<<endl;
         int row =  ch1->getChildren().at(0)->getLineNumber();
         int col =  ch1->getChildren().at(0)->getColNumber();
         string type = myTable.findSymbol(name).type;
         vector<string> tt = Spilt_type(type);
         vector<string> compare;
         //cout<<"t0="<<tt[0]<<endl;
         compare.push_back(tt[0]);
         Get_Array_Index(myTable,ch1->getChildren().at(0)->getChildren().at(0),compare);

         int true_data_size  = (tt.size()-1)/2;
         int test_data_size  = (compare.size()-1);
         /*cout<<"======tt====="<<endl;
         for(int i=0;i<tt.size();i++)
            cout<<tt[i]<<" ";
         cout<<endl;
        cout<<"======compare====="<<endl;
         for(int i=0;i<compare.size();i++)
            cout<<compare[i]<<" ";
         cout<<endl;*/
    // cout<<"true_data_size="<<true_data_size<<endl;
    // cout<<"test_data_size="<<test_data_size<<"row="<<row<<" ,col="<<col<<endl;

         //檢查使用的維度是否超越宣告的維度
         if(test_data_size>true_data_size)
         {
             fprintf(stderr,INDEX_MANY,row,col,name.c_str());
         }
         else
         {
            string array_R_type ;
            TreeNode right_Node = ch1->getChildren().at(1);
            string   R_node_type = mapping[right_Node->getNodeType()];
            if(R_node_type=="CONST")
            {
                //檢查使用的維度是否正確
                int key=true_data_size-test_data_size;
                if(key!=0)
                    fprintf(stderr,ASSIG_TYPE,row,col);
                else
                {
                    string R_Exp_type = mapping[right_Node->getExpType()];
                    if(R_Exp_type=="REAL")
                        R_Exp_type = "real";
                    else if(R_Exp_type=="INT")
                        R_Exp_type = "int";
                    if(R_Exp_type!=tt[0])
                        fprintf(stderr,ASSIG_TYPE,row,col);

                }
                
            }
            else if(R_node_type=="OP")
            {
                vector<OP_element> my_OP;
                vector<string> soldier;
                Check_OP_type(myTable,right_Node,my_OP,soldier);


               /* cout<<"====the left====="<<endl;
                cout<<type<<endl;
                cout<<"=====the OP====="<<endl;
                for(int i=0;i<my_OP.size();i++)
                    cout<<my_OP[i].op_type<<" ";
                cout<<endl;
                cout<<"====the type===="<<endl;
                for(int i=0;i<soldier.size();i++)
                    cout<<soldier[i]<<" ";
                cout<<endl;*/


                for(int i=soldier.size()-1;i>=1;i--)
                {
                     if(soldier[i]==soldier[i-1])
                        my_OP.pop_back();
                    else
                    {
                        bool key_check=false;
                        for(auto i=0u;i<soldier.size();i++)
                        {
                            if(soldier[i]==type)
                                key_check=true;
                        }
                        for(auto i=0u;i<soldier.size();i++)
                        {
                            if(Spilt_type(soldier[i])[0]!=Spilt_type(type)[0])
                                key_check=false;
                        }
                        if(!key_check)
                        {
                        int index=my_OP.size()-1;
                        fprintf(stderr,ARITH_TYPE,my_OP[index].row,my_OP[index].col,my_OP[index].op_type.c_str());
                        return ;
                        }
                    }
                }

            }

         }
      }
       
    }

    
    else if(mapping[ch1->getNodeType()]=="IF" && IF_index==0)
    {
   //  cout<<"IF"<<endl;
         
        //確認有無未宣告的變數
        Check_IF_statement(myTable,ch1,check);
        for(auto i=0u;i<ch1->getChildren().size();i++)
        {
            if(ch1->getChildren().at(i)!=NULL)
            {
                TreeNode wh1 = ch1->getChildren().at(i);
                string node_type = mapping[wh1->getNodeType()];
                string Arrtibut = mapping[wh1->getAttribute()._int];
                 if(node_type=="OP"&&check)
                {  
                  //  cout<<"hhh"<<endl;
                    vector<OP_element> my_OP;
                    vector<string> soldier;
                    Check_OP_type(myTable,wh1,my_OP,soldier);
                    for(int j=soldier.size()-1;j>=1;j--)
                    {
                        if(soldier[j]==soldier[j-1])
                            my_OP.pop_back();
                        else
                        {
                            int index=my_OP.size()-1;
                            fprintf(stderr,ARITH_TYPE,my_OP[index].row,my_OP[index].col,my_OP[index].op_type.c_str());
                        } 
                    }
                }

                else if(node_type=="ASSIGN"&&Arrtibut=="ID"&&check)
                {   
                    
                    int row_L_=0,col_L_=0,row_R_=0,col_R_=0;
                    vector<string> left_type_;
                    vector<string> right_type_;
                    SymbolTable_Element element_;
                    //檢查左值
                    if(wh1->getChildren().at(0)!=NULL)
                    {
                        element_=myTable.findSymbol(wh1->getChildren().at(0)->getAttribute()._string);
                        left_type_.push_back(Spilt_type(element_.type)[0]);
                    }

                    //檢查右值
                    if(wh1->getChildren().at(1)!=NULL)
                    {
                        TreeNode c2 = wh1->getChildren().at(1);
                        row_R_ = c2->getLineNumber();
                        col_R_ = c2->getColNumber();
                        string name = c2->getAttribute()._string;
                        string tmp_type = mapping[c2->getNodeType()];

                        //若右值為常數
                        if(tmp_type=="CONST")
                        {
                        if(mapping[c2->getExpType()]=="INT")
                            right_type_.push_back("int");
                        else if(mapping[c2->getExpType()]=="REAL")
                            right_type_.push_back("real");
                        if(right_type_!=left_type_&&check)
                            fprintf(stderr,ASSIG_TYPE,row_L_,col_L_);
                        }

                        //若右值為變數
                        else if(tmp_type=="PROC_ID")
                        {
                            element_ = myTable.findSymbol(c2->getAttribute()._string);
                            // cout<<"I'm the="<<element.name<<endl;
                            vector<string> tmp = Spilt_type(element_.type);
                            if(tmp.size()>1)
                                    right_type_.push_back(tmp[0]);
                            else
                                    right_type_ = tmp;
                            //right_type = Spilt_type(element.type);
                            if(right_type_!=left_type_&&check)
                                fprintf(stderr,ASSIG_TYPE,row_L_,col_L_);
                        }

                        //若右值為函數
                        else if(tmp_type=="PROC_FUNC")
                        {   
                            vector<string> check_para;
                            element = myTable.findSymbol(c2->getAttribute()._string);
                            right_type_ = Spilt_type(element.type);//取出該函數名存在symbol table中的type資料
                            Find_Fun_Para(myTable,c2,check_para,right_type_);
                    
                            if(check_para!=right_type_&&check)
                            {
                                fprintf(stderr,WRONG_ARGS,row_R_,col_R_,name.c_str());
                                return;
                            }
                            else if(right_type_[0]!=left_type_[0]&&check)
                                fprintf(stderr,ASSIG_TYPE,row_L_,col_L_);
                        }

                        else if(tmp_type=="OP")
                        {
                            if(check)
                            {
                                vector<OP_element> my_OP;
                                vector<string> soldier;
                                Check_OP_type(myTable,c2,my_OP,soldier);

                                  
                                for(int i=soldier.size()-1;i>=1;i--)
                                {
                                    if(soldier[i]==soldier[i-1])
                                        my_OP.pop_back();
                                    else
                                    {
                                        int index=my_OP.size()-1;
                                        fprintf(stderr,ARITH_TYPE,my_OP[index].row,my_OP[index].col,my_OP[index].op_type.c_str());
                                        return ;
                                    } 
                                }
                                right_type_.push_back(soldier[0]);
                                vector<string> tmp_right = Spilt_type(soldier[0]);
                                if(tmp_right.size()>1)
                                {
                                    if(tmp_right[0]!=left_type[0])
                                        fprintf(stderr,ASSIG_TYPE,row_L_,col_L_);
                                }
                                else
                                {
                                    if(right_type_!=left_type_)
                                        fprintf(stderr,ASSIG_TYPE,row_L_,col_L_);
                                }
                            }
                        }
                    }                  
                }
            }
        }
    }

    else if(mapping[ch1->getNodeType()]=="WHILE")
    {
      // cout<<"WHILE"<<endl;
        //確認有無未宣告的變數
        Check_IF_statement(myTable,ch1,check);
        
           // cout<<"while OP"<<endl;
            for(auto i=0u;i<ch1->getChildren().size();i++)
            {
                if(ch1->getChildren().at(i)!=NULL)
                {
                    TreeNode wh1 = ch1->getChildren().at(i);
                    string node_type = mapping[wh1->getNodeType()];
                    if(node_type=="OP")
                    {
                        
                        vector<OP_element> my_OP;
                        vector<string> soldier;
                        Check_OP_type(myTable,wh1,my_OP,soldier);
                        for(int j=soldier.size()-1;j>=1;j--)
                        {
                            if(soldier[j]==soldier[j-1])
                                my_OP.pop_back();
                            else
                            {
                                int index=my_OP.size()-1;
                                fprintf(stderr,ARITH_TYPE,my_OP[index].row,my_OP[index].col,my_OP[index].op_type.c_str());
                            } 
                        }
                    }
                    else if(node_type=="LABLE")
                    {
                        New_Label_Check(symbolTable_Q,wh1);
                        TreeNode t1 = wh1->getSibling();
                        while(t1!=NULL)
                        {
                            New_Label_Check(symbolTable_Q,t1);
                            t1=t1->getSibling();
                        }
                    }

                }
            }
    
    }
   
}

void  TreeDefine::Check_IF_statement(MySymbolTable &myTable,TreeNode root,bool &check)
{
    string name = root->getAttribute()._string;
    int    row  = root->getLineNumber();
    int    col  = root->getColNumber();
    string node_type=mapping[root->getNodeType()];
    string exp_type =mapping[root->getExpType()];
    SymbolTable_Element element;
      if(node_type=="ID"||node_type=="PROC_ID")
    {
        if(myTable.bFind(name,0,row,col))
        {
           element = myTable.findSymbol(name);
           vector<string> type = Spilt_type(element.type);
           int K_count=1;
           TreeNode t = root;
           for(auto i=0u;i<t->getChildren().size();i++)
           {
               if(t->getChildren().at(i)!=NULL)
                {
                    K_count+=1;
                   // cout<<"my child is"<<t->getChildren().at(i)->getAttribute()._string<<endl;
                }
           }
           if(static_cast<size_t>(K_count)>type.size())
            fprintf(stderr,INDEX_MANY,row,col,name.c_str());
        }
        else
        {
            check=false;
        }
        
    }
    else if(node_type=="PROC_FUNC")
    {
        if(myTable.bFind(name,1,row,col))
        {
           
        }
        else
        {
            check=false;
        }
    }
    for(auto i=0u;i<root->getChildren().size();i++)
    {
        if(root->getChildren().at(i)!=NULL)
        {
            Check_IF_statement(myTable,root->getChildren().at(i),check);
        }
    }
    TreeNode t = root->getSibling();
    if(t!=NULL)
         Check_IF_statement(myTable,t,check);
}


void  TreeDefine::Get_Array_Index(MySymbolTable &myTable,TreeNode root,vector<string> &compare_data)
{
    //起始root為陣列開頭變數的第一個child Node
    string node_type = mapping[root->getNodeType()];
    string name = root->getAttribute()._string;
    string exp_type  = mapping[root->getExpType()];
    string the_type;
    int    row  = root->getLineNumber();
    int    col  = root->getColNumber();
    if(node_type=="CONST")
    {
        //cout<<"Hello const"<<endl;
        if(exp_type=="INT")
        {
         //   cout<<"INT Value="<<root->getAttribute()._double<<endl;
            compare_data.push_back("int");
        }
        else if(exp_type=="REAL")
            fprintf(stderr,INDEX_TYPE,row,col);
    }
    else if(node_type=="PROC_ID")
    {
        name = root->getAttribute()._string;
        SymbolTable_Element element = myTable.findSymbol(name);
        if(element.type=="int")
            compare_data.push_back("int");
        else if(element.type=="real")
            fprintf(stderr,INDEX_TYPE,row,col);
    }
    else if(node_type=="OP")
    {
        /*vector<OP_element> my_OP;
        vector<string> soldier;
        Check_OP_type(myTable,root,my_OP,soldier);
        for(int i=soldier.size()-1;i>=1;i--)
        {
            if(soldier[i]==soldier[i-1])
                 my_OP.pop_back();
            else
            {
                int index=my_OP.size()-1;
                fprintf(stderr,ARITH_TYPE,my_OP[index].row,my_OP[index].col,my_OP[index].op_type.c_str());
                return ;
            } 
        }
        if(soldier[0]=="int")
            compare_data.push_back("int");
        else if(soldier[0]=="real")
            fprintf(stderr,INDEX_TYPE,row,col);*/
        node_type = mapping[root->getChildren().at(0)->getNodeType()];
        exp_type  = mapping[root->getChildren().at(0)->getExpType()];
        name = root->getChildren().at(0)->getAttribute()._string;
        if(node_type=="CONST")
        {
       //  cout<<"CONST"<<endl;
         if(exp_type=="INT")
            compare_data.push_back("int");
         else if(exp_type=="REAL")
            fprintf(stderr,INDEX_TYPE,row,col);
        }
        else if(node_type=="PROC_ID")
        {

        SymbolTable_Element element = myTable.findSymbol(name);
        if(element.type=="int")
            compare_data.push_back("int");
        else if(element.type=="real")
            fprintf(stderr,INDEX_TYPE,row,col);
        }
        else if(node_type=="OP")
        {
         //   cout<<"OP"<<endl;
            node_type = mapping[root->getChildren().at(0)->getChildren().at(0)->getNodeType()];
            exp_type  = mapping[root->getChildren().at(0)->getChildren().at(0)->getExpType()];
            name = root->getChildren().at(0)->getChildren().at(0)->getAttribute()._string;
            if(node_type=="CONST")
            {
         //   cout<<"OP CONST"<<endl;
            if(exp_type=="INT")
            {
                compare_data.push_back("int");
            }
            else if(exp_type=="REAL")
                fprintf(stderr,INDEX_TYPE,row,col);
            }
            else if(node_type=="PROC_ID")
            {
         //   cout<<"PROC_ID"<<endl;
            SymbolTable_Element element = myTable.findSymbol(name);
            if(element.type=="int")
                compare_data.push_back("int");
            else if(element.type=="real")
                fprintf(stderr,INDEX_TYPE,row,col);
            }
        }

    }
    //TreeNode t = root->getSibling();
    if(root->getSibling()!=NULL)
    {
       // cout<<"Sibling"<<endl;
        root=root->getSibling();
        Get_Array_Index(myTable,root,compare_data);
    }
    //if(node_type=="PROC_ID")
}

void  TreeDefine::Check_Array_Index(MySymbolTable &myTable,TreeNode root,bool &check)
{
    string name = root->getAttribute()._string;
    int    row  = root->getLineNumber();
    int    col  = root->getColNumber();  
    string node_type=mapping[root->getNodeType()];
    string exp_type =mapping[root->getExpType()];
    SymbolTable_Element element;
    if(node_type=="ID"||node_type=="PROC_ID")
    {
        if(myTable.bFind(name,0,row,col))
        {
            element = myTable.findSymbol(name);
            if(element.type=="real")
            {
                fprintf(stderr,INDEX_TYPE,row,col);
                check=false;
            }
        }
        else
        {
            check=false;
        }
        
    }
   // cout<<node_type<<",name="<<name<<endl;

    for(auto i=0u;i<root->getChildren().size();i++)
    {
        if(root->getChildren().at(i)!=NULL)
        {
            Check_Array_Index(myTable,root->getChildren().at(i),check);
           // if(root->getChildren().at(i)->getSibling()!=NULL)
            // Check_Array_Index(myTable,root->getChildren().at(i)->getSibling(),check);
        }
    }
    TreeNode t = root->getSibling();
    if(t!=NULL)
         Check_Array_Index(myTable,t,check);

}

string TreeDefine::Return_OP_type(TreeNode root)
{
     string op_str;
    if(mapping[root->getNodeType()]=="OP")
    {
        string op_type = mapping[root->getAttribute()._int];
        if(op_type=="PLUS")         op_str = "+";
        else if(op_type=="MINUS")   op_str = "-";
        else if(op_type=="MUL")     op_str = "*";
        else if(op_type=="DIV")     op_str = "/";
        else if(op_type=="LT")      op_str = "<";
        else if(op_type=="LE")      op_str = "<=";
        else if(op_type=="GT")      op_str = ">";
        else if(op_type=="GE")      op_str = ">=";
        else if(op_type=="EQUAL")   op_str = "=";
        else if(op_type=="UNEQUAL") op_str = "!=";

    }
    return op_str;
}

void TreeDefine::New_OP(MySymbolTable &myTable,TreeNode root,bool &check)
 {
     string node_type = mapping[root->getNodeType()];
     string exp_type  = mapping[root->getExpType()];
     string name      = root->getAttribute()._string;
     int    def       = 0;
     int    row_c     = root->getLineNumber();
     int    col_c     = root->getColNumber();
     if(node_type=="PROC_FUNC") def=1;
     if((node_type=="PROC_FUNC"||node_type=="PROC_ID"||node_type=="ID")
     &&!myTable.bFind(name,def,row_c,col_c)) 
        check=false;
     for(auto i=0u;i<root->getChildren().size();i++)
     {
         if(root->getChildren().at(i)!=NULL)
         {
             New_OP(myTable,root->getChildren().at(i),check);
             TreeNode sb = root->getChildren().at(i);
             while(sb->getSibling()!=NULL)
                {
                    New_OP(myTable,sb->getSibling(),check);
                    sb = sb->getSibling();
                } 
         }
     }
 }

void TreeDefine::Check_OP_type(MySymbolTable &myTable,TreeNode root,vector<OP_element> &my_OP, vector<string> &soldier)
 {
     OP_element OP_tmp;
     TreeNode sb = root;
     string node_type = mapping[sb->getNodeType()];
     if(node_type =="OP")
     {
       OP_tmp.op_type = Return_OP_type(sb);
       OP_tmp.row     = sb->getLineNumber();
       OP_tmp.col     = sb->getColNumber();
       my_OP.push_back(OP_tmp);
     }
     else if(node_type=="CONST")
     {
       //cout<<"CONST"<<endl;
       string exp_type = mapping[sb->getExpType()];
       if(exp_type=="INT")       
        {
            soldier.push_back("int");
        }
       else if(exp_type=="REAL") soldier.push_back("real");
     }
     else if(node_type=="PROC_ID")
     {
      //  vector<string> pp = Spilt_type(myTable.findSymbol(sb->getAttribute()._string).type);
        string pp = myTable.findSymbol(sb->getAttribute()._string).type;
        soldier.push_back(pp);
     }
     else if(node_type=="PROC_FUNC")
     {
         string pp = myTable.findSymbol(sb->getAttribute()._string).type;
        // cout<<"pp1="<<pp<<endl;
         vector<string>xs = Spilt_type(pp);
         pp = xs[0];
        // cout<<"pp2="<<pp<<endl;
         soldier.push_back(pp);
         /*if(xs[0]=="int")       soldier.push_back("int");
         else if(xs[0]=="real") soldier.push_back("real");*/
     }
     for(auto i=0u;i<root->getChildren().size();i++)
     {
         if(mapping[root->getNodeType()]!="PROC_ID"
          &&mapping[root->getNodeType()]!="PROC_FUNC"
          &&root->getChildren().at(i)!=NULL)
         {
             Check_OP_type(myTable,root->getChildren().at(i),my_OP,soldier);
         }
     }
 }

void TreeDefine:: Find_Fun_Para(MySymbolTable &myTable,TreeNode ch2,vector<string> &check_para,vector<string> right_type)
{
     check_para.push_back(right_type[0]);
     if(ch2->getChildren().at(0)!=NULL)
        {
        //第一個參數
        TreeNode ch3 = ch2->getChildren().at(0);
        if(mapping[ch3->getNodeType()]=="CONST"&&mapping[ch3->getExpType()]=="INT")
            check_para.push_back("int");
        else if(mapping[ch3->getNodeType()]=="CONST"&&mapping[ch3->getExpType()]=="REAL")
            check_para.push_back("real");
        else if(mapping[ch3->getNodeType()]=="PROC_ID"&&myTable.bFind(ch3->getAttribute()._string,0,ch3->getLineNumber(),ch3->getColNumber()))
            check_para.push_back(myTable.findSymbol(ch3->getAttribute()._string).type);
        else if(mapping[ch3->getNodeType()]=="OP")
        {
        
            if(ch3->getChildren().at(0)!=NULL)
            {
                if(mapping[ch3->getChildren().at(0)->getNodeType()]=="CONST"
                &&mapping[ch3->getChildren().at(0)->getExpType()]=="INT")
                    check_para.push_back("int");
                else if(mapping[ch3->getChildren().at(0)->getNodeType()]=="CONST"
                    &&mapping[ch3->getChildren().at(0)->getExpType()]=="REAL")
                    check_para.push_back("real");
            }
        }
                     //後續其他的參數
                     /*TreeNode t = ch3->getSibling();
                     if(t!=NULL)
                     {
                        if(mapping[t->getNodeType()]=="CONST"&&mapping[t->getExpType()]=="INT")
                           check_para.push_back("int");
                        else if(mapping[t->getNodeType()]=="CONST"&&mapping[t->getExpType()]=="REAL")
                           check_para.push_back("real");
                        else if(mapping[t->getNodeType()]=="PROC_ID"&&myTable.bFind(t->getAttribute()._string,0,t->getLineNumber(),t->getColNumber()))
                           check_para.push_back(myTable.findSymbol(t->getAttribute()._string).type);
                     }*/
        while(ch3->getSibling()!=NULL)
        {
            ch3=ch3->getSibling();
            if(mapping[ch3->getNodeType()]=="CONST"&&mapping[ch3->getExpType()]=="INT")
            check_para.push_back("int");
            else if(mapping[ch3->getNodeType()]=="CONST"&&mapping[ch3->getExpType()]=="REAL")
            check_para.push_back("real");
            else if(mapping[ch3->getNodeType()]=="PROC_ID"&&myTable.bFind(ch3->getAttribute()._string,0,ch3->getLineNumber(),ch3->getColNumber()))
            check_para.push_back(myTable.findSymbol(ch3->getAttribute()._string).type);
        }
    }
}




void TreeDefine::Check_fake_func_para(MySymbolTable &myTable,TreeNode root)
{
    int row = root->getLineNumber();
    int col = root->getColNumber();
    string node_type =mapping[root->getNodeType()];
    string name = root->getAttribute()._string;
    if(node_type=="PROC_ID")
      //  bool bb = myTable.bFind(name,0,row,col);
         myTable.bFind(name,0,row,col);
    TreeNode t = root->getSibling();
    if(t!=NULL)
        Check_fake_func_para(myTable,t);
}

vector<string> TreeDefine::Spilt_type(string type)
{
    vector<string> output;
    string tmp = "";
    bool tt=true;
    for (auto i = 0u; i < type.size(); i++)
    {
        if (type[i] == ' ' 
         || type[i] == '['
         || type[i] == ']' 
         || type[i] == '~'
         || type[i] == '('
         || type[i] == ')'
         || type[i] == ',')
        {
            if (!tmp.empty())
                output.push_back(tmp);
            tt = false;
            tmp = "";
        }
        else {
            tmp += type[i];
        }
    }
    if (tt)
        output.push_back(tmp);
    return output;
}


void TreeDefine::FunPro_decl(MySymbolTable &symbolTable_Q,TreeNode root,int scope)
{ 
    bool para=false;
    bool suc_defunc=true;
    MySymbolTable subTable;     //  variable in function
    subTable.Setparent(&symbolTable_Q);
    vector<string> subShow;     //  the order of showing sub variable
    string  sub_type;           //  store the type message of sub variable
    SymbolTable_Element element;

    //new para struct
    vector<para_element> para_data;
    para_element tmp_para;
    string node_type;
    string my_ID;
    int row,col;
    TreeNode tmp_node;
    if(mapping[root->getChildren().at(0)->getNodeType()]=="PROCEDUREHEAD"
    || mapping[root->getChildren().at(0)->getNodeType()]=="FUNCTIONHEAD")
    { 
         my_ID = root->getChildren().at(0)->getAttribute()._string;
         row   = root->getChildren().at(0)->getLineNumber();
         col   = root->getChildren().at(0)->getColNumber();
    }
    
    tmp_node = root->getChildren().at(0);

    if(tmp_node->getChildren().size()==1)
     {  
         node_type +="void";
     }
    
    else if(mapping[tmp_node->getChildren().at(1)->getExpType()]=="INT")
    {
        node_type +="int";
    }
    else if(mapping[tmp_node->getChildren().at(1)->getExpType()]=="REAL")
    {
        node_type +="real";
    }
    else if(mapping[tmp_node->getChildren().at(1)->getExpType()]=="STRING")
    {
        node_type +="string";
    }
    if(tmp_node->getChildren().at(0)!=NULL && mapping[tmp_node->getChildren().at(0)->getNodeType()]=="VAL_PARA")
    {
        TreeNode tmp_node2=tmp_node->getChildren().at(0);
        TreeNode tmp_node3=tmp_node2->getChildren().at(0);
        //new para struct
        tmp_para.para_id =tmp_node3->getAttribute()._string;
        if(mapping[tmp_node2->getChildren().at(1)->getNodeType()]=="SIMPLE_SYS")
        {
            string tmp;
            if(mapping[tmp_node2->getChildren().at(1)->getExpType()]=="INT")          tmp="int";
            else if (mapping[tmp_node2->getChildren().at(1)->getExpType()]=="REAL")   tmp="real";
            else if (mapping[tmp_node2->getChildren().at(1)->getExpType()]=="STRING") tmp="string";
            sub_type = tmp;
            //new para struct
            tmp_para.para_TYPE = tmp;
            
            int count=1;
            tmp_node2=tmp_node2->getChildren().at(0);
            while(tmp_node2->getSibling()!=NULL)
            {
                count++;
                tmp_node2 = tmp_node2->getSibling();
            }
            string tmp2=tmp;
            for(int i=1;i<count;i++)
            {

                tmp2+=", ";
                tmp2+=tmp;
            }
            node_type+=" (";
            node_type+=tmp2;
            para=true;
        }
        else if(mapping[tmp_node2->getChildren().at(1)->getNodeType()]=="ARRAY")
        {
            vector<int> index;
            TreeNode tmp_node3 = tmp_node2->getChildren().at(1);
            while(mapping[tmp_node3->getNodeType()]=="ARRAY")
            {
                for(auto i=0u;i<tmp_node3->getChildren().at(0)->getChildren().size();i++)
                {
                    index.push_back(tmp_node3->getChildren().at(0)->getChildren().at(i)->getAttribute()._double);
                }
                tmp_node3= tmp_node3->getChildren().at(1);
            }
            string check = mapping[tmp_node3->getExpType()];
            if(check == "INT")       check ="int";
            else if(check=="REAL")   check ="real";
            else if(check=="STRING") check ="string";
            sub_type = check;
            
            string array_index;
            for(auto i=0u;i<index.size();i+=2)
            {
              array_index+="[";
              array_index+=to_string(index[i]);
              array_index+="~";
              array_index+=to_string(index[i+1]);
              array_index+="]";

              sub_type+="[";
              sub_type+=to_string(index[i]);
              sub_type+="~";
              sub_type+=to_string(index[i+1]);
              sub_type+="]";
            }
            node_type+=" (";
            node_type+=check;
            node_type+=array_index;
            para=true;

            //new para struct
            tmp_para.para_TYPE =sub_type;
        }
        //new para struct
        para_data.push_back(tmp_para);

        while(tmp_node3->getSibling())
        {
            //new para struct
            tmp_para.para_id = tmp_node3->getSibling()->getAttribute()._string;
            para_data.push_back(tmp_para);
            tmp_node3 = tmp_node3->getSibling();
        }

        //deal with val para Sibling node
        TreeNode subPara = tmp_node->getChildren().at(0); //VAL_PARA
        if(subPara->getSibling()!=NULL) node_type+=", ";
        while(subPara->getSibling()!=NULL)
        {
            if (mapping[subPara->getSibling()->getNodeType()] == "VAL_PARA")
            {
                 TreeNode tmp2=subPara->getSibling();
                 TreeNode tmp3=tmp2->getChildren().at(0);
                 tmp_para.para_id = tmp3->getAttribute()._string;
                 if(mapping[tmp2->getChildren().at(1)->getNodeType()]=="SIMPLE_SYS")
                 {
                     string tmp;
                     if(mapping[tmp2->getChildren().at(1)->getExpType()]=="INT")          tmp="int";
                     else if (mapping[tmp2->getChildren().at(1)->getExpType()]=="REAL")   tmp="real";
                     else if (mapping[tmp2->getChildren().at(1)->getExpType()]=="STRING") tmp="string";
                     sub_type = tmp;

                     tmp_para.para_TYPE=tmp;

                     int count=1;
                     tmp2=tmp2->getChildren().at(0);
                     while(tmp2->getSibling()!=NULL)
                     {
                         count++;
                         tmp2 = tmp2->getSibling();
                     }
                     string t2=tmp;
                     for(int i=1;i<count;i++)
                     {
                         t2+=", ";
                         t2+=tmp;
                     }
                     node_type+=t2;
                     para=true;
                 }
                 else if(mapping[tmp2->getChildren().at(1)->getNodeType()]=="ARRAY")
                 {
                     vector<int> index;
                     TreeNode arr_node = tmp2->getChildren().at(1);
                     while(mapping[arr_node->getNodeType()]=="ARRAY")
                     {
                         for(auto i=0u;i<arr_node->getChildren().at(0)->getChildren().size();i++)
                         {
                             index.push_back(arr_node->getChildren().at(0)->getChildren().at(i)->getAttribute()._double);
                         }
                         arr_node = arr_node->getChildren().at(1);
                     }
                     string check=mapping[arr_node->getExpType()];
                     if(check=="INT")  check="int";
                     else if(check=="REAL")   check ="real";
                     else if(check=="STRING") check ="string";
                     sub_type = check;

                     string array_index;
                    for(auto i=0u;i<index.size();i+=2)
                    {
                        array_index+="[";
                        array_index+=to_string(index[i]);
                        array_index+="~";
                        array_index+=to_string(index[i+1]);
                        array_index+="]";

                        sub_type+="[";
                        sub_type+=to_string(index[i]);
                        sub_type+="~";
                        sub_type+=to_string(index[i+1]);
                        sub_type+="]";
                    }
                    node_type+=check;
                    node_type+=array_index;
                    para=true;
                    tmp_para.para_TYPE =sub_type;
                 }
                para_data.push_back(tmp_para);
            }
            subPara=subPara->getSibling();
        }
    
    }

    if(para) node_type+=")";
    element.name  = my_ID;
    element.scope = scope;
    element.type  = node_type;
    element.K_Type = "FUNCTION";
    element.row   = row;
    element.col   = col;

    //加入symbol table成功
    if(symbolTable_Q.addSymbol(element.name,element))
    {
        SHOW_NEWSYM(element.name);  
        Global_Show.push_back(element.name);
    }
    //加入symbol table失敗
    else
    {
        string bug_name="";
        root->getChildren().at(0)->setAttribute("");

        suc_defunc=false;
    }
    SHOW_NEWSCP();
    scope+=1;

    for(auto i=0u;i<para_data.size();i++)
    {
        element.name = para_data[i].para_id;
        element.scope = scope;
        element.type = para_data[i].para_TYPE;
        if(subTable.addSymbol(element.name,element))
        {
            SHOW_NEWSYM(element.name);  
            subShow.push_back(element.name);
        }

    }
    

   
    //The ROUTINEHEAD of FUNCTIOIN/PROCEDURE
    tmp_node = root->getChildren().at(1);  
    bool has_return = false;
    
     //加入symbol table成功,檢查函數有無返回值
    if(mapping[root->getNodeType()]=="FUNCTION"&&tmp_node->getChildren().at(2)!=NULL)
    {
        Label_Check_Name(tmp_node->getChildren().at(2),my_ID,has_return);
        if(!has_return)
        {
           // fprintf(stderr,"First\n");
          //  cout<<"11111"<<endl;
          if(function_noreturn_show)
            fprintf(stderr,RETURN_VAL,row,col,my_ID.c_str());
          else
            function_noreturn_show=true;
        }
    }
    //加入symbol table成功,但函數內無Label
    else if(mapping[root->getNodeType()]=="FUNCTION"&&tmp_node->getChildren().at(2)==NULL)
       {
           fprintf(stderr,RETURN_VAL,row,col,my_ID.c_str());
       }
    
    
    for(auto i=0u;i<tmp_node->getChildren().size();i++)
    {
        if(tmp_node->getChildren().at(i)!=NULL)
        {
            //handle VAR of Function/Procedure
            if(mapping[tmp_node->getChildren().at(i)->getNodeType()]=="VAR")
            {
                Sub_Var_decl(subTable,tmp_node->getChildren().at(i),scope,subShow );
                TreeNode           t1=tmp_node->getChildren().at(i);
                while(t1->getSibling()!=NULL)
                {
                    Sub_Var_decl(subTable,t1->getSibling(),scope,subShow );
                    t1=t1->getSibling();
                }
            }
            //handle LABEL of Function/Procedure
            
            else if(mapping[tmp_node->getChildren().at(i)->getNodeType()]=="LABEL")
            {
               New_Label_Check(subTable,tmp_node->getChildren().at(i),suc_defunc,row,col);
                TreeNode t2=tmp_node->getChildren().at(i);
                while(t2->getSibling()!=NULL)
                {
                    if(mapping[t2->getSibling()->getNodeType()]=="LABEL")
                       New_Label_Check(subTable,t2->getSibling(),suc_defunc,row,col);
                    t2=t2->getSibling();
                }
            }
        }
    }
 SHOW_CLSSCP();
 SHOW_SYMTAB_HEAD();
 Show_data(subTable,subShow);
 Show_data(symbolTable_Q,Global_Show);
 SHOW_SYMTAB_TAIL();
}

void TreeDefine::Label_Check_Name(TreeNode label_node,string fun_name,bool &has_return)
{
     if(label_node->getAttribute()._string==fun_name)
    {
        has_return=true;
    }
    for(auto i=0u;i<label_node->getChildren().size();i++){
        if(label_node->getChildren().at(i)!=NULL)
        {
           Label_Check_Name(label_node->getChildren().at(i),fun_name,has_return);
        }
       
    }
    TreeNode t  =label_node->getSibling();
    if(t!=NULL)
     Label_Check_Name(t,fun_name,has_return);
}

void TreeDefine::Sub_Var_decl(MySymbolTable &subTable,TreeNode root,int scope ,vector<string> &subShow)
{
   SymbolTable_Element element;
   vector<string> my_ID;
   vector<int> my_row;
   vector<int> my_col;

   //add child ID
   if(mapping[root->getChildren().at(0)->getNodeType()]=="ID")
    {
        my_ID.push_back(root->getChildren().at(0)->getAttribute()._string);
        my_row.push_back(root->getChildren().at(0)->getLineNumber());
        my_col.push_back(root->getChildren().at(0)->getColNumber());

        /*
        cout<<"ID:"<<root->getChildren().at(0)->getAttribute()._string<<
        ", row:"<<root->getChildren().at(0)->getLineNumber()<<", col:"<<
        root->getChildren().at(0)->getColNumber()<<endl;
        */
    }
   
   TreeNode tmp = root->getChildren().at(0);
   //add child sibling id
   while(tmp->getSibling()!=NULL)
   {
       if(mapping[tmp->getSibling()->getNodeType()]=="ID")
        {
            my_ID.push_back(tmp->getSibling()->getAttribute()._string);
            my_row.push_back(tmp->getSibling()->getLineNumber());
            my_col.push_back(tmp->getSibling()->getColNumber());

            /*
            cout<<"ID:"<<tmp->getSibling()->getAttribute()._string<<
            ", row:"<<tmp->getSibling()->getLineNumber()<<", col:"<<
            tmp->getSibling()->getColNumber()<<endl;
            */
        }
       tmp = tmp->getSibling();
   }

   //type check and insert symbol table
   TreeNode tmp_2 = root->getChildren().at(1);
   string exp_type = mapping[tmp_2->getExpType()];
   if(exp_type!="ARRAY")
   {
    if(exp_type=="INT") exp_type="int";
    else if(exp_type=="REAL") exp_type="real";
    else if(exp_type=="STRING") exp_type="string";
    for(auto i=0u;i<my_ID.size();i++)
     {
       element.name = my_ID[i];
       element.scope = scope;
       element.type = exp_type;
       element.row = my_row[i];
       element.col = my_col[i];
       element.K_Type = "variable";
      // cout<<"ID:"<<my_ID[i]<<", row="<<my_row[i]<<", col="<<my_col[i]<<", i="<<i<<endl;
       if(subTable.addSymbol(element.name,element))
       {
           SHOW_NEWSYM(element.name);  
           subShow.push_back(element.name);
       }
     }
   }
   else
   {
   vector<int>array_index;
   while(exp_type=="ARRAY"&&tmp_2!=NULL)
   {
     double t1 =tmp_2->getChildren().at(0)->getChildren().at(0)->getAttribute()._double;
     double t2 =tmp_2->getChildren().at(0)->getChildren().at(1)->getAttribute()._double;
     array_index.push_back(t1);
     array_index.push_back(t2);
     exp_type = mapping[tmp_2->getChildren().at(1)->getExpType()];
     tmp_2 =            tmp_2->getChildren().at(1);
   }
   if(exp_type=="INT") exp_type="int";
   else if(exp_type=="REAL") exp_type="real";
   else if(exp_type=="STRING") exp_type="string";
   string stmp=exp_type;

   for(int i=array_index.size()-1;i>=0;i-=2)
   {
    stmp+="[";
    stmp+=to_string(array_index[i-1]);
    stmp+="~";
    stmp+=to_string(array_index[i]);
    stmp+="]";
   }
   for(auto i=0u;i<my_ID.size();i++)
     {
       element.name = my_ID[i];
       element.scope = scope;
       element.type = stmp;
       element.row = my_row[i];
       element.col = my_col[i];
       element.K_Type = "variable_array";
     //  cout<<"ID:"<<my_ID[i]<<", row="<<my_row[i]<<", col="<<my_col[i]<<", i="<<i<<endl;
       if(subTable.addSymbol(element.name,element))
       {
           SHOW_NEWSYM(element.name);  
           subShow.push_back(element.name);
       }
     }
   }
}

void TreeDefine::print_K(TreeNode root, int level)
{
   if(level == 0)
        if(root == NULL) return;
   
   cout<<" "<<mapping[root->getNodeType()];
   cout<<" "<<mapping[root->getExpType()];
   switch(root->getAttribute().flag)
    {
        case 1: cout << " " << root->getAttribute()._int; break;
        case 2: cout << " " << root->getAttribute()._double; break;
        case 3: cout << " " << root->getAttribute()._string; break;
        case 4: cout << " " << mapping[root->getAttribute()._int]; break;
        default: cout << " "<< mapping[0]; break;
    }
    cout<<endl;
    for(auto i=0u;i<root->getChildren().size();i++){
        if(root->getChildren().at(i) != NULL){
            cout<<mapping[root->getChildren().at(i)->getNodeType()]<<endl;
            print_K(root->getChildren().at(i), 0);
        }
    }
    TreeNode t = root->getSibling();
    if(t != NULL)
    {
        print_K(t, 0);
    }
}

void TreeDefine::Print_Test(TreeNode root)
{
   /* cout<<"Test0:"<<root->getAttribute()._string<<endl;
    cout<<"Test1:"<<root->getChildren().at(0)->getAttribute()._string<<endl;
    cout<<"Test1-1:"<<mapping[root->getChildren().at(0)->getNodeType()]<<endl;
    cout<<"Test1-2:"<<mapping[root->getChildren().at(0)->getExpType()]<<endl;
    cout<<"Test1-3:"<<mapping[root->getChildren().at(1)->getNodeType()]<<endl;*/
   // cout<<"Test2:"<<root->getChildren().at(0)->getChildren().at(0)->getAttribute()._string<<endl;//a
   // cout<<"Test3:"<<mapping[root->getChildren().at(0)->getChildren().at(1)->getExpType()]<<endl;
  /*  cout<<"Test3:"<<root->getChildren().at(0)->getChildren().at(0)->getSibling()->getAttribute()._string<<endl;//b
    cout<<"Test4:"<<root->getChildren().at(0)->getChildren().at(0)->getSibling()->getSibling()->getAttribute()._string<<endl;
    cout<<"Test5:"<<mapping[root->getChildren().at(0)->getChildren().at(1)->getExpType()]<<endl;//INT
    cout<<"Test6:"<<mapping[root->getChildren().at(0)->getSibling()->getNodeType()]<<endl; //VAR
    cout<<"Test7:"<<root->getChildren().at(0)->getSibling()->getChildren().at(0)->getAttribute()._string<<endl; //d
    cout<<"Test8:"<<root->getChildren().at(0)->getSibling()->getChildren().at(0)->getSibling()->getAttribute()._string<<endl; //e
    cout<<"Test9:"<<mapping[root->getChildren().at(0)->getSibling()->getChildren().at(1)->getNodeType()]<<endl; //ARRAY
    cout<<"Test10:"<<mapping[root->getChildren().at(0)->getSibling()->getChildren().at(1)->getChildren().at(0)->getNodeType()]<<endl;//SIMPLE_LIMIT
    cout<<"Test10-1:"<<mapping[root->getChildren().at(0)->getSibling()->getChildren().at(1)->getChildren().at(1)->getNodeType()]<<endl;
    cout<<"Test10-2:"<<mapping[root->getChildren().at(0)->getSibling()->getChildren().at(1)->getChildren().at(1)->getExpType()]<<endl;//SIMPLE_LIMIT
    cout<<"Test11:"<<root->getChildren().at(0)->getSibling()->getChildren().at(1)->getChildren().at(0)->getChildren().at(0)->getAttribute()._double<<endl;//1
    cout<<"Test12:"<<root->getChildren().at(0)->getSibling()->getChildren().at(1)->getChildren().at(0)->getChildren().at(1)->getAttribute()._double<<endl;//1
  */

    
    /*cout<<"Test0:"<<          root->getChildren().at(1)->getChildren().at(0)->getAttribute()._string<<endl;
    cout<<"Test0-1:"<<mapping[root->getChildren().at(1)->getChildren().at(1)->getNodeType()]<<endl;  //ROUTINEHEAD
    cout<<"Test0-2:"<<mapping[root->getChildren().at(1)->getChildren().at(1)->getChildren().at(0)->getNodeType()]<<endl;//VAR
    cout<<"Test0-3:"<<root->getChildren().at(1)->getChildren().at(1)->getChildren().at(0)->getChildren().at(0)->getSibling()->getAttribute()._string<<endl;
    cout<<"Test1:"<<mapping[root->getChildren().at(1)->getChildren().at(0)->getNodeType()]<<endl;
    cout<<"Test2:"<<mapping[root->getChildren().at(1)->getChildren().at(0)->getChildren().at(0)->getNodeType()]<<endl;
    cout<<"Test3:"<<mapping[root->getChildren().at(1)->getChildren().at(0)->getChildren().at(0)->getChildren().at(0)->getNodeType()]<<endl;
    cout<<"Test4:"<<mapping[root->getChildren().at(1)->getChildren().at(0)->getChildren().at(0)->getChildren().at(1)->getNodeType()]<<endl;
    cout<<"Test5:"<< mapping[root->getChildren().at(1)->getChildren().at(1)->getChildren().at(2)->getNodeType()]<<endl;//LABEL
    cout<<"Test5-1:"<< mapping[root->getChildren().at(1)->getChildren().at(1)->getChildren().at(2)->getChildren().at(0)->getNodeType()]<<endl;
    cout<<"Test5-2:"<< mapping[root->getChildren().at(1)->getChildren().at(1)->getChildren().at(2)->getChildren().at(0)->getChildren().at(1)->getNodeType()]<<endl;//OP
    cout<<"Test5-3:"<< mapping[root->getChildren().at(1)->getChildren().at(1)->getChildren().at(2)->getChildren().at(0)->getChildren().at(0)->getNodeType()]<<endl;//ID
    cout<<"Test5-4:"<<         root->getChildren().at(1)->getChildren().at(1)->getChildren().at(2)->getChildren().at(0)->getChildren().at(0)->getAttribute()._string<<endl;
    cout<<"Test5-5:"<<         root->getChildren().at(1)->getChildren().at(1)->getChildren().at(2)->getChildren().at(0)->getChildren().at(1)->getChildren().at(1)->getAttribute()._string<<endl;//OP

    cout<<"Test6:"<< mapping[root->getChildren().at(1)->getChildren().at(1)->getChildren().at(2)->getSibling()->getNodeType()]<<endl;
    cout<<"Test7:"<< mapping[root->getChildren().at(1)->getChildren().at(1)->getChildren().at(2)->getSibling()->getNodeType()]<<endl;//LABEL
    cout<<"Test8:"<< mapping[root->getChildren().at(1)->getChildren().at(1)->getChildren().at(2)->getSibling()->getSibling()->getNodeType()]<<endl;//LABEL

    cout<<"Test9:"<<mapping[root->getChildren().at(1)->getSibling()->getNodeType()]<<endl;*/
  //  cout<<"Test1:"<<mapping[root->getChildren().at(2)->getSibling()->getChildren().at(0)->getNodeType()]<<endl;
}

