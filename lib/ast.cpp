
#include  "../include/ast.h"

using namespace std;

//====Global Variable====
extern FILE *Jfile;

bool function_noreturn_show = true;

string ProgramName;

int BlockCount=1;

vector<string> Global_Show; //store global variable, function ID
//======================

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

void TreeDefine::Show_data_codegen(MySymbolTable K_symbolTable,vector<string> ctr)
{
  string c1; //name 
  string c2; //type
  string c3; //K_type
  SymbolTable_Element element;
   for(auto i=0u;i<ctr.size();i++)
    {
        string key = ctr[i];
        element = K_symbolTable.findSymbol(key);
        c1 = element.name;
        //d1 = element.scope;
        c2 = element.type;
        c3 = element.K_Type;
        if(c3=="variable" )
        {
            vector<string> ttt=Spilt_type(c2);
            if(ttt[0]=="int")
            {
                fprintf(Jfile,"    ldc 0\n");
                string strr="    putstatic "+ProgramName+"/"+c1+" I\n";
                fprintf(Jfile,"%s",strr.c_str());
            }
            else if(ttt[0]=="real")
            {
                fprintf(Jfile,"    ldc 0.0\n");
                string strr="    putstatic "+ProgramName+"/"+c1+" F\n";
                fprintf(Jfile,"%s",strr.c_str());
            }
            else if(ttt[0]=="char")
            {
                fprintf(Jfile,"    ldc \"\"\n");
                string strr="    putstatic "+ProgramName+"/"+c1+" Ljava/lang/String;\n";
                fprintf(Jfile,"%s",strr.c_str());
            }
        }
        else if(c3=="variable_array")
        {
            vector<string> ttt=Spilt_type(c2);
            string type;
            if(ttt[0]=="int") type="I";
            else if(ttt[0]=="real") type="F";
            else if(ttt[0]=="char") type="Ljava/lang/String;";

            string output;
            vector<int> index;
            for(int i=ttt.size()-1;i>0;i=i-2)
            {
                int n1 = stoi(ttt[i]);
               // int n2 = stoi(ttt[i-1]);
                //===!!!====//
                //index.push_back(n1-n2+1);
                index.push_back(n1);
            }
            for(auto i=0u;i<index.size();i++)
            {
                output=output+"[";
                string tmp = to_string(index[i]);
                tmp="    ldc "+tmp;
                fprintf(Jfile,"%s\n",tmp.c_str());
            }
            string output1="    multianewarray "+output+type+" "+to_string(index.size());
            fprintf(Jfile,"%s\n",output1.c_str());
            string output2="    putstatic "+ProgramName+"/"+c1+" "+output+type;
            fprintf(Jfile,"%s\n",output2.c_str());

        }
       
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
void TreeDefine::CreateBasicFun()
{
    string tt="";
    tt=".method public static writelnI(I)V";
    fprintf(Jfile,"%s\n",tt.c_str());
    tt=".limit locals 5";
    fprintf(Jfile,"%s\n",tt.c_str());
    tt=".limit stack 5";
    fprintf(Jfile,"%s\n",tt.c_str());
    tt="getstatic java/lang/System/out Ljava/io/PrintStream;";
    fprintf(Jfile,"%s\n",tt.c_str());
    tt="iload 0";
    fprintf(Jfile,"%s\n",tt.c_str());
    tt="invokevirtual java/io/PrintStream/println(I)V";
    fprintf(Jfile,"%s\n",tt.c_str());
    tt="return";
    fprintf(Jfile,"%s\n",tt.c_str());
    tt=".end method";
    fprintf(Jfile,"%s\n",tt.c_str());
}
void TreeDefine::theCreate(TreeNode root)
{
    string tmp;
    bool   bHasLabel=false;
    if(root!=NULL)
    { 
        SHOW_NEWSCP();
        string sym = root->getAttribute()._string;
        ProgramName = sym;
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
    //CreateBasicFun();
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
                //====for code generation====//
                bHasLabel=true;
                if(bHasLabel)
                {
                  
                    tmp=".method public static main([Ljava/lang/String;)V\n";
                    fprintf(Jfile,"%s",tmp.c_str());
                    int size = symbolTable_Q.symbolMap.size();
                 
                    if(size<100)
                    {
                        tmp=".limit locals 50\n.limit stack 50\n";
                        fprintf(Jfile,"%s",tmp.c_str());
                    }
                    else
                    {
                        tmp=".limit locals 100\n.limit stack 100\n";
                        fprintf(Jfile,"%s",tmp.c_str());
                    }

                    Show_data_codegen(symbolTable_Q,Global_Show);
                }
                
                //==========================//
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

string TreeDefine::Return_Array_type(string name,string type)
{
    vector<string> t = Spilt_type(type);
    string type2;
    if(t[0]=="int") type2="I";
    else if(t[0]=="real") type2="F";
    else if(t[0]=="char") type2="Ljava/lang/String;";
    string output;
     for(int i=t.size()-1;i>0;i=i-2)
     {
         output=output+"[";
     }
     return (output+type2);
}


void TreeDefine::Find_Fun_jj(MySymbolTable &myTable,TreeNode root)
{
    string node_type=mapping[root->getNodeType()];
    if(node_type=="PROC_ID"||node_type=="ID")
    {
        string name=root->getAttribute()._string;
        int row =   root->getLineNumber();
        int col =   root->getColNumber();

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

void TreeDefine::ShowLDC(string exp_type,TreeNode ch,int &isflaot)
{
    string tmp1="    ldc ";
    if(exp_type=="INT")
    {
        isflaot=0;
        int myInt = static_cast<int>(ch->getAttribute()._double);
        string str = to_string(myInt);
        tmp1 = tmp1+ str+ "\n";
        fprintf(Jfile,"%s",tmp1.c_str());
    }
    else if(exp_type=="REAL")
    {
        isflaot=1;
        string str = to_string(ch->getAttribute()._double);
        tmp1 = tmp1+ str+ "\n";
        fprintf(Jfile,"%s",tmp1.c_str());
    }
    else if(exp_type=="STRING")
    {
        isflaot=2;
        string str = ch->getAttribute()._string;
        tmp1 = tmp1+ str+ "\n";
        fprintf(Jfile,"%s",tmp1.c_str());
    }
}

 void TreeDefine::Op_ss(string optype,int &BlockCount,bool Notfault)
 {
     string block_sts = "L"+to_string(BlockCount);
     string sts01     = optype+block_sts+"\n";
     fprintf(Jfile,"%s",sts01.c_str());
     if(Notfault)
        fprintf(Jfile,"    ldc 1\n");
     else
        fprintf(Jfile,"    ldc 0\n");
     string block_sts2 = "    goto L"+to_string(BlockCount+1)+"\n";
     fprintf(Jfile,"%s",block_sts2.c_str());
     fprintf(Jfile,"%s:\n",block_sts.c_str());
     if(Notfault)
        fprintf(Jfile,"    ldc 0\n");
     else
        fprintf(Jfile,"    ldc 1\n");
     BlockCount = BlockCount+1;
     block_sts = "L"+to_string(BlockCount);
     BlockCount = BlockCount+1;
     fprintf(Jfile,"%s:\n",block_sts.c_str());
 }

string TreeDefine::WR_name(string name)
{
     if      (name == "writelnI")  return "    invokestatic io/writelnI(I)V\n";
     else if (name == "writelnR")  return "    invokestatic io/writelnR(F)V\n";
     else if (name == "writelnS")  return "    invokestatic io/writelnS(Ljava/lang/String;)V\n";
     return "";
}
void TreeDefine::WRITEandREAD(MySymbolTable &myTable,TreeNode node)
{
    int isfloat=0;
    bool stringVV=true;
    string func_name  = node->getAttribute()._string; //writelnI or writelnR or writelnS
    string the_name   = WR_name(func_name);

        if(node->getChildren().at(0)!=NULL)
        {
            TreeNode ch = node->getChildren().at(0);
            string node_type = mapping[ch->getNodeType()];
            string exp_type  = mapping[ch->getExpType()];

            //若內建函數writeln..等等的參數為const
            if(node_type=="CONST")
            {
                ShowLDC(exp_type,ch,isfloat);
                fprintf(Jfile,"%s",the_name .c_str());
            }
            //若內建函數writeln..等等的參數為ID
            else if(node_type=="ID")
            {

            }
            //若內建函數writeln..等等的參數為OP
            else if(node_type=="OP")
            {
                string optype = mapping[ch->getAttribute()._int];
                for(auto i=0u;i<ch->getChildren().size();i++)
                {
                    if(ch->getChildren().at(i)!=NULL)
                    {
                        TreeNode gg = ch->getChildren().at(i);
                        string exp_type_2 = mapping[gg->getExpType()];
                        string node_type_2 = mapping[gg->getNodeType()];
                        if(optype=="PLUS"&&exp_type_2=="STRING"&&stringVV)
                        {
                            stringVV=false;
                            fprintf(Jfile,"new java/lang/StringBuffer\n");
                            fprintf(Jfile,"dup\n");
                            fprintf(Jfile,"invokespecial java/lang/StringBuffer/<init>()V\n");
                            
                        }
                        ShowLDC(exp_type_2,gg,isfloat);
                        if(optype=="PLUS"&&exp_type_2=="STRING")
                            fprintf(Jfile,"invokevirtual java/lang/StringBuffer/append(Ljava/lang/String;)Ljava/lang/StringBuffer;\n");
                        
                        else if(node_type_2=="PROC_ID")
                        {
                            SymbolTable_Element me = myTable.findSymbol(gg->getAttribute()._string);
                            if(me.K_Type=="variable_array")
                            {
                                // fprintf(Jfile,";<OPPPP__2>\n");
                                vector<P_element> Pe;
                                vector<E_element> Ee;
                                int level=1;
                                Codegen_OP_2(myTable,ch,Pe,Ee,level);
                                sort(Ee.begin(), Ee.end(), compareByLevel_E);
                                int E_size=(int)Ee.size();
                                string result=Ee[E_size-1].et;
                                for(int i=E_size-2;i>=0;--i)
                                {
                                    result = result+"\n"+Ee[i].et;
                                    result = result+"\n"+Pe[i].OP;
                                }
                                fprintf(Jfile,"%s\n",result.c_str());
                                fprintf(Jfile,"%s",the_name.c_str());
                                return;
                            }
                            else if(me.type=="char")
                            {
                               string cc= "    aload "+me.name;
                               fprintf(Jfile,"%s\n",cc.c_str());
                               fprintf(Jfile,"invokevirtual java/lang/StringBuffer/append(Ljava/lang/String;)Ljava/lang/StringBuffer;\n");
                            //    string str="    ldc \"rabbit\"";
                            //    fprintf(Jfile,"%s\n",str.c_str());
                            //    fprintf(Jfile,"invokevirtual java/lang/StringBuffer/append(Ljava/lang/String;)Ljava/lang/StringBuffer;\n");
                            }
                        
                        }
                    }
                }
                
                if(optype=="PLUS")
                {
                    if(isfloat==1) 
                        fprintf(Jfile,"    fadd\n");
                    else if(isfloat==2)
                    {
                         fprintf(Jfile,"invokevirtual java/lang/StringBuffer/toString()Ljava/lang/String;\n");
                    }
                    else  if(isfloat==0)         
                        fprintf(Jfile,"    iadd\n");
                    fprintf(Jfile,"%s",the_name .c_str());
                }
                else if(optype=="MINUS")
                {
                    if(ch->getChildren().at(1)==NULL)
                        fprintf(Jfile,"    fneg\n");
                    else
                    {
                        if(isfloat==1) 
                            fprintf(Jfile,"    fsub\n");
                        else        
                            fprintf(Jfile,"    isub\n");
                    }
                    fprintf(Jfile,"%s",the_name .c_str());
                }
                else if(optype=="MUL")
                {
                    if(isfloat==1) fprintf(Jfile,"    fmul\n");
                    else        fprintf(Jfile,"    imul\n");
                    fprintf(Jfile,"%s",the_name .c_str());
                }
                else if(optype=="DIV")
                {
                    if(isfloat==1) fprintf(Jfile,"    fdiv\n");
                    else        fprintf(Jfile,"    idiv\n");
                    fprintf(Jfile,"%s",the_name .c_str());
                }
                else if(optype=="GT")
                {
                    if(isfloat==1)
                    {
                        fprintf(Jfile,"    fcmpg\n");
                        fprintf(Jfile,"    ldc 1\n");
                        Op_ss("    if_icmpeq ",BlockCount);
                    }
                    else
                        Op_ss("    if_icmpgt ",BlockCount);
                    fprintf(Jfile,"%s",the_name .c_str());
                }
                else if(optype=="LT")
                {
                    if(isfloat==1)
                    {
                        fprintf(Jfile,"    fcmpg\n");
                        fprintf(Jfile,"    ldc -1\n");
                        Op_ss("    if_icmpeq ",BlockCount);
                    }
                    else
                        Op_ss("    if_icmplt ",BlockCount);
                    fprintf(Jfile,"%s",the_name .c_str());
                }
                else if(optype=="EQUAL")
                {
                    if(isfloat==1)
                    {
                        fprintf(Jfile,"    fcmpg\n");
                        fprintf(Jfile,"    ldc 0\n");
                        Op_ss("    if_icmpeq ",BlockCount);
                    }
                    else
                        Op_ss("    if_icmpeq ",BlockCount);
                    fprintf(Jfile,"%s",the_name .c_str());
                }
                else if(optype=="LE")
                {
                    if(isfloat==1)
                    {
                        fprintf(Jfile,"    fcmpg\n");
                        fprintf(Jfile,"    ldc 1\n");
                        Op_ss("    if_icmplt ",BlockCount);
                    }
                    else
                        Op_ss("    if_icmple ",BlockCount);
                    fprintf(Jfile,"%s",the_name .c_str());
                }
                else if(optype=="GE")
                {
                    if(isfloat==1)
                    {
                        fprintf(Jfile,"    fcmpg\n");
                        fprintf(Jfile,"    ldc -1\n");
                        Op_ss("    if_icmpgt ",BlockCount);
                    }
                    else
                        Op_ss("    if_icmpge ",BlockCount);
                    fprintf(Jfile,"%s",the_name .c_str());
                }
                else if(optype=="UNEQUAL")
                {
                    if(isfloat==1)
                    {
                        fprintf(Jfile,"    fcmpg\n");
                        fprintf(Jfile,"    ldc 0\n");
                        Op_ss("    if_icmpne ",BlockCount);
                    }
                    else
                        Op_ss("    if_icmpne ",BlockCount);
                    fprintf(Jfile,"%s",the_name .c_str());
                }
                else if(optype=="NOT")
                {
                     Op_ss("    ifne ",BlockCount,true);
                     fprintf(Jfile,"%s",the_name .c_str());
                }

            }
            else if(node_type=="PROC_ID")
            {
              // fprintf(Jfile,"; ===PROC_ID====\n");
               string str;
               if(ch->getAttribute()._string=="readlnI")
               {
                   str="    invokestatic io/readlnI()I\n";
                   fprintf(Jfile,"%s",str.c_str());
                   fprintf(Jfile,"%s",the_name.c_str());
               }
               else
               {
                   string sts3;
                   string sts4;
                   SymbolTable_Element SE = myTable.findSymbol(ch->getAttribute()._string);
                   if(SE.K_Type=="variable")
                   {
                        if(SE.type=="int")
                        {
                            sts3="    getstatic "+ProgramName+"/"+SE.name+" I\n";
                            sts4="    invokestatic io/writelnI(I)V\n";
                        }
                        else if(SE.type=="real")
                        {
                            sts3="    getstatic "+ProgramName+"/"+SE.name+" F\n";
                            sts4="    invokestatic io/writelnR(F)V\n";
                        }
                        else if(SE.type=="char")
                        {
                            sts3="    getstatic "+ProgramName+"/"+SE.name+" Ljava/lang/String;\n";
                            sts4="    invokestatic io/writelnS(Ljava/lang/String;)V\n";
                        }
                        fprintf(Jfile,"%s",sts3.c_str());
                        fprintf(Jfile,"%s",sts4.c_str());
                   }
                   else if(SE.K_Type=="variable_array")
                   {
                       string ss = Return_Array_type(SE.name,SE.type);
                       ss = "    getstatic "+ProgramName+"/"+SE.name+" "+ss;
                       fprintf(Jfile,"%s\n",ss.c_str());

                       for(auto i=0u;i<ch->getChildren().size();i++)
                       {
                           if(ch->getChildren().at(i)!=NULL)
                           {
                               TreeNode t=ch->getChildren().at(i);
                               string t_node = mapping[t->getNodeType()];
                               if(t_node=="CONST")
                               {
                                   int n1=static_cast<int>(t->getAttribute()._double);
                                   string s1="    ldc "+to_string(n1);
                                   fprintf(Jfile,"%s\n",s1.c_str());
                                   fprintf(Jfile,"    ldc 1\n");
                                   fprintf(Jfile,"    isub\n");
                               }
                                while(t->getSibling()!=NULL)
                                {
                                    t=t->getSibling();
                                    string node_type2 = mapping[t->getNodeType()];
                                    if(node_type2=="CONST")
                                    {
                                    int n1 = static_cast<int>(t->getAttribute()._double);
                                    string s1 = "    ldc "+to_string(n1);
                                    fprintf(Jfile,"    aaload\n");
                                    fprintf(Jfile,"%s\n",s1.c_str());
                                    fprintf(Jfile,"    ldc 1\n");
                                    fprintf(Jfile,"    isub\n");
                                    }
                                }

                           }
                       }
                        fprintf(Jfile,"    iaload\n");
                        fprintf(Jfile,"%s",the_name.c_str());

                   }
                   else if(SE.K_Type=="FUNCTION")
                   {
                     string fun_decl_ = Return_F_Name(SE.name,SE.type);
                     string oo="    invokestatic "+ProgramName+"/"+fun_decl_+"\n";
                     
                     fprintf(Jfile,"%s",oo.c_str());
                     
                     fprintf(Jfile,"%s",the_name.c_str());
                   }

               }
            }
            else if(node_type=="PROC_FUNC")
            {
                string output= the_name;
              // string output= "invokevirtual java/io/PrintStream/println(I)V";
                SymbolTable_Element SE = myTable.findSymbol(ch->getAttribute()._string);
                string fun_decl = Return_F_Name(SE.name,SE.type);

                output="    invokestatic "+ProgramName+"/"+fun_decl+"\n"+output;

                for(auto i=0u;i<ch->getChildren().size();i++)
                {
                    if(ch->getChildren().at(i)!=NULL)
                    {
                        TreeNode cc = ch->getChildren().at(i);
                        if(mapping[cc->getNodeType()]=="PROC_ID")
                        {
                            SymbolTable_Element SE1 = myTable.findSymbol(cc->getAttribute()._string);
                            if(SE1.K_Type=="FUNCTION")
                            {
                                string fun_decl_ = Return_F_Name(SE1.name,SE1.type);
                                output="    invokestatic "+ProgramName+"/"+fun_decl_+"\n"+output;
                            }
                            else if(SE1.K_Type=="variable")
                            {
                                string ff="    getstatic "+ProgramName+"/"+SE1.name;
                                if(SE1.type=="int") ff=ff+" I";
                                else if(SE1.type=="real") ff=ff+" F";
                                output = ff+"\n"+output;
                            }
                        }  
                    }
                }

                fprintf(Jfile,"%s",output.c_str());

            }
        }

}

string TreeDefine::Return_F_Name(string name,string type)
{
    string fun_decl = name+"(";
    vector<string> f_type = Spilt_type(type);
    for(auto i=1u;i<f_type.size();i++)
        {
            if(f_type[i]=="int")       fun_decl+="I";
            else if(f_type[i]=="real") fun_decl+="F";
            
        }
    fun_decl+=")";
    if(f_type[0]=="int")       fun_decl+="I";
    else if(f_type[0]=="real") fun_decl+="F";
    else if(f_type[0]=="void") fun_decl+="V";
    return fun_decl;
}


void  TreeDefine::Return_F_para(MySymbolTable &myTable,TreeNode root,string &output)
{
    string node_type=mapping[root->getNodeType()];
    string exp_type =mapping[root->getExpType()];
    if(node_type=="PROC_FUNC")
    {
        SymbolTable_Element ee=myTable.findSymbol(root->getAttribute()._string);
        string fun="    invokestatic "+ProgramName+"/"+Return_F_Name(ee.name,ee.type);
        output = fun+"\n"+output;
    }
    else if(node_type=="CONST")
    {
        if(exp_type=="INT")
        {
            int myInt=static_cast<int>(root->getAttribute()._double);
            string II="    ldc "+to_string(myInt);
            output = II+"\n"+output;
        }
    }
    for(auto i=0u;i<root->getChildren().size();i++)
    {
        if(root->getChildren().at(i)!=NULL)
        {
            Return_F_para(myTable,root->getChildren().at(i),output);
        }
    }
}

void TreeDefine::New_Label_Check(MySymbolTable &myTable,TreeNode root,bool suc_defunc,int ROW,int COL,int IF_index,string func_name)
{
    //====for test====//
   // fprintf(Jfile,"LABELLLL\n");
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
        string f_name = ch1->getAttribute()._string;
        if(f_name == "writelnI" ||
           f_name == "writelnR" ||
           f_name == "writelnS")
        {
            WRITEandREAD(myTable,ch1);
            return ;
        }
    
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
                    else if(mapping[ch3->getNodeType()]=="CONST"&&mapping[ch3->getExpType()]=="STRING")
                        check_para.push_back("string");
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
                        else if(mapping[ch3->getNodeType()]=="CONST"&&mapping[ch3->getExpType()]=="STRING")
                           check_para.push_back("string");
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
            else
            {
                string fun_d=Return_F_Name(element.name,element.type);
                string oo="    invokestatic "+ProgramName+"/"+fun_d+"\n";
                fprintf(Jfile,"%s",oo.c_str());
            }
        }
    }
    
    //檢查一般ID assign
    else if(mapping[ch1->getNodeType()]=="ASSIGN"&&mapping[ch1->getAttribute()._int]=="ID")
    {
       
       //======codegen======//
        string output="";
        bool func_assign=false;
       //===================//

       //檢查ASSIGN左值
       if(ch1->getChildren().at(0)!=NULL)
       {
           row_L = ch1->getChildren().at(0)->getLineNumber();
           col_L = ch1->getChildren().at(0)->getColNumber();
          
          if(myTable.bFind(ch1->getChildren().at(0)->getAttribute()._string,0,row_L,col_L))
          {
            element = myTable.findSymbol(ch1->getChildren().at(0)->getAttribute()._string);
            left_type.push_back(Spilt_type(element.type)[0]);
            if(element.name!=func_name)
            {
                if(element.type=="int")
                {
                    output = "    putstatic "+ProgramName+"/"+element.name+" I\n";
                }
                else if(element.type=="real")
                {
                    output = "    putstatic "+ProgramName+"/"+element.name+" F\n";
                }
                else if(element.type=="char")
                {
                     output = "    putstatic "+ProgramName+"/"+element.name+" Ljava/lang/String;\n";
                }
            }
            else
            {
                func_assign=true;
            }
            
            //檢查重複加入symboltable的函數
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
           //===codegen===//
           string code_tmp="";
          // int isfloat = 0;
           //=============//
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
               { 
                   right_type.push_back("int");
                   int theInt= static_cast<int>(ch2->getAttribute()._double);
                   string Int= to_string(theInt);
                   output = "    ldc "+Int+"\n"+output;
               }
               else if(mapping[ch2->getExpType()]=="REAL")
               { 
                   right_type.push_back("real");
                   string Real= to_string(ch2->getAttribute()._double);
                   output = "    ldc "+Real+"\n"+output;
               }
               else if(mapping[ch2->getExpType()]=="STRING")
               {
                   string mystr=ch2->getAttribute()._string;
                   output = "    ldc "+mystr+"\n"+output;
               }
              if(right_type!=left_type&&check&&suc_defunc)
                 fprintf(stderr,ASSIG_TYPE,row_L,col_L);
            //  ShowLDC(mapping[ch2->getExpType()],ch2,isfloat);
            fprintf(Jfile,"%s",output.c_str());
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
                  
                  //=====codegen===//
                  string r_exp_type;
                  if(!func_assign)
                  {
                    string tmp1="    getstatic "+ProgramName+"/"+element.name;
                    r_exp_type=element.type;
                    if(r_exp_type=="int")         tmp1= tmp1+" I\n";
                    else if(r_exp_type=="real")   tmp1= tmp1+" F\n";

                    if(element.K_Type=="FUNCTION")
                    {
                       string fun_decl_ = Return_F_Name(element.name,element.type);
                       tmp1="    invokestatic "+ProgramName+"/"+fun_decl_+"\n";
                    }
                    
                    output = tmp1+output;
                    fprintf(Jfile,"%s",output.c_str());
                  }
                  else if(element.name=="b")
                  {
                      fprintf(Jfile,"    getstatic test_global2/b I\n");
                  }
                  else
                  {
                    r_exp_type=element.type;
                    // fprintf(Jfile,"r_exp_type=%s\n",r_exp_type.c_str());
                    // fprintf(Jfile,"name=%s\n",element.name.c_str());
                    if(r_exp_type=="int")         fprintf(Jfile,"    iload 0\n");
                    else if(r_exp_type=="real")   fprintf(Jfile,"    iload 0.0\n");
                  }
                  //===============//
               } 
               else if(ch2->getAttribute()._string=="readlnI")    
               {
                   string tmp1="    invokestatic io/readlnI()I\n";
                   output = tmp1+output;
                   fprintf(Jfile,"%s",output.c_str());
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
                   bool funS=true;
                   /*
                   fprintf(Jfile,"check:\n");
                       for(int i=0;i<check_para.size();i++)
                            fprintf(Jfile,"%s\n",check_para[i].c_str());
                       fprintf(Jfile,"right:\n");
                       for(int i=0;i<right_type.size();i++)
                            fprintf(Jfile,"%s\n",right_type[i].c_str());*/
                  
                   if(check_para!=right_type&&check)
                   {
                       fprintf(stderr,WRONG_ARGS,row_R,col_R,name.c_str());
                       funS=false;
                       return;
                   }
                   else if(right_type[0]!=left_type[0]&&check&&suc_defunc)
                   { 
                       
                       fprintf(stderr,ASSIG_TYPE,row_L,col_L);
                       funS=false;
                   }
                   //funS=true;
                   //=====for codegen=====//
                   if(funS)
                   {
                       string fun_cc="    invokestatic "+ProgramName+"/"+Return_F_Name(element.name,element.type);
                       output = fun_cc+"\n"+output;
                       for(auto i=0u;i<ch2->getChildren().size();i++)
                       {
                           if(ch2->getChildren().at(i)!=NULL)
                           {
                               TreeNode c22=ch2->getChildren().at(i);
                               string cc2_nodetype=mapping[c22->getNodeType()];
                               string cc2_exptype =mapping[c22->getExpType()];
                               if(cc2_nodetype=="CONST")
                               {
                                   if(cc2_exptype=="INT")
                                   {
                                       int myInt = static_cast<int>(c22->getAttribute()._double);
                                       string out_I="    ldc "+to_string(myInt);
                                     //  fprintf(Jfile,"%s\n",out_I.c_str());
                                       output = out_I+"\n"+output;
                                   }
                               }
                               else if(cc2_nodetype=="PROC_FUNC")
                               {
                                   Return_F_para(myTable,c22,output);
                               }
                           }
                       }
                     //  fprintf(Jfile,"for testttt\n");
                       fprintf(Jfile,"%s",output.c_str());
                   }
                   //====================//

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
          //  fprintf(Jfile,"OPPPPPP\n");
             New_OP(myTable,ch2,check);
             if(check)
             {
                vector<OP_element> my_OP;
                vector<string> soldier;
                Check_OP_type(myTable,ch2,my_OP,soldier);
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
                //=====for codegen=====
                //fprintf(Jfile,"for testttt\n");
                string pp="";
                vector<string> OP_Type;
                vector<string> et;
               // Codegen_OP(myTable,ch2,OP_Type,et,pp);

               //===for test=====//
               vector<P_element> Pe;
               vector<E_element> Ee;
               int level=1;
               Codegen_OP_2(myTable,ch2,Pe,Ee,level);
               sort(Ee.begin(), Ee.end(), compareByLevel_E);
               int E_size=(int)Ee.size();
               string result=Ee[E_size-1].et;
               for(int i=E_size-2;i>=0;--i)
               {
                   result = result+"\n"+Ee[i].et;
                   result = result+"\n"+Pe[i].OP;
               }

                fprintf(Jfile,"%s\n",result.c_str());
                fprintf(Jfile,"%s",output.c_str());
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
      if(check)
      {
        //ch1->getChildren().at(0)為陣列開頭變數
         bool check2=true;
         string name = ch1->getChildren().at(0)->getAttribute()._string;

         int row =  ch1->getChildren().at(0)->getLineNumber();
         int col =  ch1->getChildren().at(0)->getColNumber();
         string type = myTable.findSymbol(name).type;
         vector<string> tt = Spilt_type(type);
         vector<string> compare;

         compare.push_back(tt[0]);
         Get_Array_Index(myTable,ch1->getChildren().at(0)->getChildren().at(0),compare);

         int true_data_size  = (tt.size()-1)/2;
         int test_data_size  = (compare.size()-1);

    

         //檢查使用的維度是否超越宣告的維度
         if(test_data_size>true_data_size)
         {
             fprintf(stderr,INDEX_MANY,row,col,name.c_str());
             check2=false;
         }
         else
         {
            

            //===================//
            string array_R_type ;
            TreeNode right_Node = ch1->getChildren().at(1);
            string   R_node_type = mapping[right_Node->getNodeType()];

            

            if(R_node_type=="CONST")
            {
                //檢查使用的維度是否正確
                int key=true_data_size-test_data_size;
                if(key!=0)
                {
                    fprintf(stderr,ASSIG_TYPE,row,col);
                    check2=false;
                }
                else
                {
                    string R_Exp_type = mapping[right_Node->getExpType()];
                    if(R_Exp_type=="REAL")
                        R_Exp_type = "real";
                    else if(R_Exp_type=="INT")
                        R_Exp_type = "int";
                    if(R_Exp_type!=tt[0])
                    {
                         fprintf(stderr,ASSIG_TYPE,row,col);
                         check2=false;
                    }

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
                        check2=false;
                        return ;
                        }
                    }
                }

            }
            //====for codegen====//
            if(check2)
            {
                TreeNode left_node = ch1->getChildren().at(0);
                DealwithArraygen(myTable,left_node,right_Node);
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
                    bool the_OP_is_legal=true;
                    vector<OP_element> my_OP;
                    vector<string> soldier;
                    Check_OP_type(myTable,wh1,my_OP,soldier);
                    for(int j=soldier.size()-1;j>=1;j--)
                    {
                        if(soldier[j]==soldier[j-1])
                            my_OP.pop_back();
                        else
                        {
                            the_OP_is_legal = false;
                            int index=my_OP.size()-1;
                            fprintf(stderr,ARITH_TYPE,my_OP[index].row,my_OP[index].col,my_OP[index].op_type.c_str());
                        } 
                    }
                    if(the_OP_is_legal)
                    {
                    //=====for codegen=====//
                        vector<string> OP_Type;
                        vector<string> et;
                        string pp;
                        Codegen_OP(myTable,wh1,OP_Type,et,pp);
                        for(auto i=0u;i<et.size();i++)
                            fprintf(Jfile,"%s\n",et[i].c_str());
                        for(auto i=0u;i<OP_Type.size();i++)
                        { 
                            string tmpp=OP_Type[i]+"L"+to_string(BlockCount);
                            fprintf(Jfile,"%s\n",tmpp.c_str());
                            BlockCount++;
                            tmpp="    goto L"+to_string(BlockCount);
                            fprintf(Jfile,"%s\n",tmpp.c_str());
                        }
                    //=====================//  
                    }
                }

                else if(node_type=="ASSIGN"&&Arrtibut=="ID"&&check)
                {   
                    //====for codegen====//
                    string fkk="L"+to_string(BlockCount-1);
                    fprintf(Jfile,"%s:\n",fkk.c_str());
                    //===================//
                    bool isfunName=false;
                    int row_L_=0,col_L_=0,row_R_=0,col_R_=0;
                    vector<string> left_type_;
                    vector<string> right_type_;
                    SymbolTable_Element element_;
                    //檢查左值
                    if(wh1->getChildren().at(0)!=NULL)
                    {
                        element_=myTable.findSymbol(wh1->getChildren().at(0)->getAttribute()._string);
                        left_type_.push_back(Spilt_type(element_.type)[0]);

                        if(element_.name==func_name)
                        {
                            isfunName=true;
                        }
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
                            {
                                right_type_.push_back("int");
                                if(isfunName)
                                {
                                    int myInt=static_cast<int>(c2->getAttribute()._double);
                                    string kk="    ldc "+to_string(myInt);
                                    fprintf(Jfile,"%s\n",kk.c_str());
                                    fprintf(Jfile,"    goto Lf\n");
                                }
                            }
                            else if(mapping[c2->getExpType()]=="REAL")
                            {
                                right_type_.push_back("real");
                                string kk="    ldc "+to_string(c2->getAttribute()._double);
                                fprintf(Jfile,"%s\n",kk.c_str());
                                fprintf(Jfile,"    goto Lf\n");
                            }
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
                            else
                            {
                               if(isfunName)
                               {
                                   if(element_.K_Type=="PARAMETERS")
                                   {
                                      
                                    string type="    iload_"+to_string(element_.para_index);
                                    fprintf(Jfile,"%s\n",type.c_str());
                                    fprintf(Jfile,"    goto Lf\n");
                                    BlockCount++;
                                   }
                               }
                            }
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

                                //=====for codegen====//
                                vector<string> OP_Type;
                                vector<string> et;
                                string pp="";
                                Codegen_OP(myTable,c2,OP_Type,et,pp);
                                for(auto i=0u;i<et.size();i++)
                                    fprintf(Jfile,"%s\n",et[i].c_str());
                                string outputt="";
                                for(auto i=0u;i<OP_Type.size();i++)
                                    outputt = OP_Type[i]+"\n"+outputt;
                                fprintf(Jfile,"%s",outputt.c_str());
                                fprintf(Jfile,"    goto Lf\n");

                                //====================//
                            }
                        }
                    }                  
                }
                //====codegen======
                else if(node_type=="IF")
                {
                    for(auto i=0u;i<wh1->getChildren().size();i++)
                    {
                        if(wh1->getChildren().at(i)!=NULL)
                        {
                            TreeNode wh2=wh1->getChildren().at(i);
                            string wh2_nodetype=mapping[wh2->getNodeType()];
                            if(wh2_nodetype=="OP")
                            {
                                string fkk="L"+to_string(BlockCount);
                                fprintf(Jfile,"%s:\n",fkk.c_str());
                                BlockCount++;
                                vector<string> OP_Type;
                                vector<string> et;
                                string pp="";
                                Codegen_OP(myTable,wh2,OP_Type,et,pp);
                                for(auto i=0u;i<et.size();i++)
                                    fprintf(Jfile,"%s\n",et[i].c_str());
                                for(auto i=0u;i<OP_Type.size();i++)
                                { 
                                    string tmpp=OP_Type[i]+"L"+to_string(BlockCount);
                                    fprintf(Jfile,"%s\n",tmpp.c_str());
                                    BlockCount++;
                                    tmpp="    goto L"+to_string(BlockCount);
                                    fprintf(Jfile,"%s\n",tmpp.c_str());
                                }
                            }
                            else if(wh2_nodetype=="ASSIGN")
                            {
                                string fkk="L"+to_string(BlockCount-1);
                                fprintf(Jfile,"%s:\n",fkk.c_str());
                                BlockCount++;
                                SymbolTable_Element wh2_element;
                                bool isfunN=false;
                                if(wh2->getChildren().at(0)!=NULL)
                                {
                                    wh2_element=myTable.findSymbol(wh2->getChildren().at(0)->getAttribute()._string);
                                    if(wh2_element.name==func_name) isfunN=true;
                                }
                                if(wh2->getChildren().at(1)!=NULL)
                                {
                                    TreeNode cc2=wh2->getChildren().at(1);
                                    string name=cc2->getAttribute()._string;
                                    string cc2_nodetype=mapping[cc2->getNodeType()];
                                    string cc2_exptype =mapping[cc2->getExpType()];
                                    string kk;
                                    if(cc2_nodetype=="CONST")
                                    {
                                        if(isfunN)
                                        {
                                            if(cc2_exptype=="INT")
                                                kk="    ldc "+to_string(static_cast<int>(cc2->getAttribute()._double));
                                            else if(cc2_exptype=="REAL")
                                                kk="    ldc "+to_string(cc2->getAttribute()._double);
                                            fprintf(Jfile,"%s\n",kk.c_str());
                                            fprintf(Jfile,"    goto Lf\n");
                                        }
                                    }
                                    else if(cc2_nodetype=="OP")
                                    {
                                        vector<string> OP_Type;
                                        vector<string> et;
                                        string pp="";
                                        Codegen_OP(myTable,cc2,OP_Type,et,pp);
                                        /*
                                        for(auto i=0u;i<et.size();i++)
                                            fprintf(Jfile,"%s\n",et[i].c_str());
                                        for(auto i=0u;i<OP_Type.size();i++) 
                                            fprintf(Jfile,"%s\n",OP_Type[i].c_str());
                                            */
                                        int op_size=OP_Type.size()-1;
                                        for(auto i=0u;i<et.size();i+=2)  
                                        {
                                            fprintf(Jfile,"%s\n",et[i].c_str());
                                            fprintf(Jfile,"%s\n",et[i+1].c_str());
                                            fprintf(Jfile,"%s\n",OP_Type[op_size-i].c_str());
                                            fprintf(Jfile,"%s\n",OP_Type[op_size-i-1].c_str());
                                        }
                                        fprintf(Jfile,"%s\n",OP_Type[0].c_str());
                                        fprintf(Jfile,"    goto Lf\n");
                                    }
                                }
                            }
                        }
                    }
                }
                else if(node_type=="PROC_ID")
                {
                  //  fprintf(Jfile,"for Proc_ID testt\n");
                
                  if(i<ch1->getChildren().size()-1)
                  {
                    int f_count=BlockCount-1;
                    string fkk = "L"+to_string(f_count)+":";
                    fprintf(Jfile,"%s\n",fkk.c_str());
                    WRITEandREAD(myTable,wh1);
                    BlockCount++;
                    string tmpp="    goto L"+to_string(BlockCount);
                    fprintf(Jfile,"%s\n",tmpp.c_str());
                  }
                  else if(i==ch1->getChildren().size()-1)
                  {
                    int f_count=BlockCount-1;
                    string fkk = "L"+to_string(f_count)+":";
                    fprintf(Jfile,"%s\n",fkk.c_str());
                    WRITEandREAD(myTable,wh1);
                    fkk = "L"+to_string(BlockCount)+":";
                    fprintf(Jfile,"%s\n",fkk.c_str());
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
        int the_return_index;
        // cout<<"while OP"<<endl;
        for(auto i=0u;i<ch1->getChildren().size();i++)
        {
            if(ch1->getChildren().at(i)!=NULL)
            {
                TreeNode wh1 = ch1->getChildren().at(i);
                string node_type = mapping[wh1->getNodeType()];
                if(node_type=="OP")
                {
                    bool the_OP_is_legal=true;
                    vector<OP_element> my_OP;
                    vector<string> soldier;
                    Check_OP_type(myTable,wh1,my_OP,soldier);
                    for(int j=soldier.size()-1;j>=1;j--)
                    {
                        if(soldier[j]==soldier[j-1])
                            my_OP.pop_back();
                        else
                        {
                            the_OP_is_legal = false;
                            int index=my_OP.size()-1;
                            fprintf(stderr,ARITH_TYPE,my_OP[index].row,my_OP[index].col,my_OP[index].op_type.c_str());
                        } 
                    }
                    if(the_OP_is_legal)
                    {
                        //=====for codegen=====//
                        vector<string> OP_Type;
                        vector<string> et;
                        string pp="";
                        Codegen_OP(myTable,wh1,OP_Type,et,pp);
                        the_return_index=BlockCount;
                        string fkk = "L"+to_string(the_return_index)+":";
                        fprintf(Jfile,"%s\n",fkk.c_str());
                        BlockCount++;
                        for(auto i=0u;i<et.size();i++)
                            fprintf(Jfile,"%s\n",et[i].c_str());
                        for(auto i=0u;i<OP_Type.size();i++)
                        { 
                            string tmpp=OP_Type[i]+"L"+to_string(BlockCount);
                            fprintf(Jfile,"%s\n",tmpp.c_str());
                            BlockCount++;
                            tmpp="    goto L"+to_string(BlockCount);
                            fprintf(Jfile,"%s\n",tmpp.c_str());
                        }
                    }
                }

                //====There has a bug====//
                else if(node_type=="LABEL")
                {
                    int ss = BlockCount-1;
                    string fkk = "L"+to_string(ss)+":";
                    fprintf(Jfile,"%s\n",fkk.c_str());
                    for(auto i=0u;i<wh1->getChildren().size();i++)
                    {
                        if(wh1->getChildren().at(i)!=NULL)
                        {
                            TreeNode ww=wh1->getChildren().at(i);
                            string ww_nodeType=mapping[ww->getNodeType()];
                            if(ww_nodeType=="PROC_ID")
                            {
                                string f_name=ww->getAttribute()._string;
                                if(f_name == "writelnI" ||
                                   f_name == "writelnR" ||
                                   f_name == "writelnS")
                                   {
                                     WRITEandREAD(myTable,ww);
                                    //  return ;
                                   }
                            }
                            else if(ww_nodeType=="ASSIGN")
                            {
                                New_Label_Check(myTable,wh1);
                            }
                        }
                    }
                    while(wh1->getSibling()!=NULL)
                    {
                        wh1=wh1->getSibling();
                        New_Label_Check(myTable,wh1);
                        
                    }
                    string tmpp="    goto L"+to_string(the_return_index);
                    fprintf(Jfile,"%s\n",tmpp.c_str());

                   /* New_Label_Check(symbolTable_Q,wh1);
                    TreeNode t1 = wh1->getSibling();
                    while(t1!=NULL)
                    {
                        New_Label_Check(symbolTable_Q,t1);
                        t1=t1->getSibling();
                    }*/
                }

            }
        }
    string fkk = "L"+to_string( BlockCount)+":";
    fprintf(Jfile,"%s\n",fkk.c_str());
    }
   
}

 void  TreeDefine::DealwithArraygen(MySymbolTable &myTable,TreeNode left_node,TreeNode right_node)
 {
     SymbolTable_Element left = myTable.findSymbol(left_node->getAttribute()._string);
     string c1 = left.name;
     string c2 = left.type;
     vector<string> t=Spilt_type(c2);
     string type;
     if(t[0]=="int") type="I";
     else if(t[0]=="real") type="F";
     else if(t[0]=="char") type="Ljava/lang/String;";
     string output;
     for(int i=t.size()-1;i>0;i=i-2)
     {
         output=output+"[";
     }
     output="    getstatic "+ProgramName+"/"+c1+" "+output+type;
     fprintf(Jfile,"%s\n",output.c_str());
     for(auto i = 0u;i<left_node->getChildren().size();i++)
     {
         if(left_node->getChildren().at(i)!=NULL)
         {
             TreeNode c = left_node->getChildren().at(i);
             string node_type = mapping[c->getNodeType()];
             if(node_type=="CONST")
             {
                 int n1 = static_cast<int>(c->getAttribute()._double);
                 string s1 = "    ldc "+to_string(n1);
                 fprintf(Jfile,"%s\n",s1.c_str());
                 fprintf(Jfile,"    ldc 1\n");
                 fprintf(Jfile,"    isub\n");
             }
             else if(node_type=="OP")
             {
                fprintf(Jfile,";Im OPP1\n");
                vector<P_element> Pe;
                vector<E_element> Ee;
                int level=1;
                Codegen_OP_2(myTable,c,Pe,Ee,level);
              
                sort(Ee.begin(), Ee.end(), compareByLevel_E);
              
                int E_size=(int)Ee.size();
                string result=Ee[E_size-1].et;
                for(int i=E_size-2;i>=0;--i)
                {
                    result = result+"\n"+Ee[i].et;
                    result = result+"\n"+Pe[i].OP;
                }
                fprintf(Jfile,"%s\n",result.c_str());
            //  fprintf(Jfile,"%s",output.c_str());
                fprintf(Jfile,"    ldc 1\n");
                fprintf(Jfile,"    isub\n");
             }
             while(c->getSibling()!=NULL)
             {
                 c=c->getSibling();
                 string node_type2 = mapping[c->getNodeType()];
                 string exp_type2  = mapping[c->getExpType()];
                 if(node_type2=="CONST")
                 {
                 int n1 = static_cast<int>(c->getAttribute()._double);
                 string s1 = "    ldc "+to_string(n1);
                 fprintf(Jfile,"    aaload\n");
                 fprintf(Jfile,"%s\n",s1.c_str());
                 fprintf(Jfile,"    ldc 1\n");
                 fprintf(Jfile,"    isub\n");
                 }
                 else if(node_type2=="OP")
                 {
                    fprintf(Jfile,";Im OPP2\n");
                    fprintf(Jfile,"    aaload\n");
                    vector<P_element> Pe;
                    vector<E_element> Ee;
                    int level=1;
                    Codegen_OP_2(myTable,c,Pe,Ee,level);
                   
                    sort(Ee.begin(), Ee.end(), compareByLevel_E);
                   
                    int E_size=(int)Ee.size();
                    string result=Ee[E_size-2].et;
                    
                    for(int i=E_size-2;i>=0;--i)
                    {
                        //====!!!====//
                        if(i==E_size-2)
                            result = result+"\n"+Ee[i+1].et;
                        else
                            result = result+"\n"+Ee[i].et;
                        result = result+"\n"+Pe[i].OP;
                    }
                    fprintf(Jfile,"%s\n",result.c_str());
                //  fprintf(Jfile,"%s",output.c_str());
                    fprintf(Jfile,"    ldc 1\n");
                    fprintf(Jfile,"    isub\n");
                 }
             }
         }
     }


     string r_node_type=mapping[right_node->getNodeType()];
     if(r_node_type=="CONST")
     {
         int n2= static_cast<int>(right_node->getAttribute()._double);
         string s2="    ldc "+to_string(n2);
         fprintf(Jfile,"%s\n",s2.c_str());
         fprintf(Jfile,"    iastore\n");

     }
     else if(r_node_type=="PROC_ID")
     {
    //    fprintf(Jfile,";<PROC_ID>\n");
       Deal_with_Array(myTable,right_node);
       fprintf(Jfile,"    iaload\n");
       fprintf(Jfile,"    iastore\n");
     }
     else if(r_node_type=="OP")
     {
        // fprintf(Jfile,";<OPPPP>\n");
        vector<P_element> Pe;
        vector<E_element> Ee;
        int level=1;
        Codegen_OP_2(myTable,right_node,Pe,Ee,level);
        
        sort(Ee.begin(), Ee.end(), compareByLevel_E);
       
        int E_size=(int)Ee.size();
        string result=Ee[E_size-1].et;
        for(int i=E_size-2;i>=0;--i)
        {
            result = result+"\n"+Ee[i].et;
            result = result+"\n"+Pe[i].OP;
        }
        fprintf(Jfile,"%s\n",result.c_str());
      //  fprintf(Jfile,"%s",output.c_str());
        fprintf(Jfile,"    iastore\n");
        // fprintf(Jfile,"; OP End\n");
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
    if((node_type=="ID"||node_type=="PROC_ID")&&
        name!="writelnI"&&
        name!="writelnR"&&
        name!="writelnS")
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
        //fprintf(Jfile,"I'm OPPP\n");
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

void  TreeDefine::Deal_with_Array(MySymbolTable &myTable,TreeNode root)
{
    SymbolTable_Element m=myTable.findSymbol(root->getAttribute()._string);
    string c1=m.name;
    string c2=m.type;
    vector<string> t=Spilt_type(c2);
    string type;
    if(t[0]=="int") type="I";
    else if(t[0]=="real") type="F";
    else if(t[0]=="char") type="Ljava/lang/String;";
    string output;
    for(int i=t.size()-1;i>0;i=i-2)
    {
        output=output+"[";
    }
    output="    getstatic "+ProgramName+"/"+c1+" "+output+type;
    fprintf(Jfile,"%s\n",output.c_str());
    for(auto i = 0u;i<root->getChildren().size();i++)
    {
        if(root->getChildren().at(i)!=NULL)
        {
            TreeNode c = root->getChildren().at(i);
            string node_type = mapping[c->getNodeType()];
            if(node_type=="CONST")
            {
                int n1 = static_cast<int>(c->getAttribute()._double);
                string s1 = "    ldc "+to_string(n1);
                fprintf(Jfile,"%s\n",s1.c_str());
                fprintf(Jfile,"    ldc 1\n");
                fprintf(Jfile,"    isub\n");
            }
            while(c->getSibling()!=NULL)
            {
                c=c->getSibling();
                string node_type2 = mapping[c->getNodeType()];
                if(node_type2=="CONST")
                {
                int n1 = static_cast<int>(c->getAttribute()._double);
                string s1 = "    ldc "+to_string(n1);
                fprintf(Jfile,"    aaload\n");
                fprintf(Jfile,"%s\n",s1.c_str());
                fprintf(Jfile,"    ldc 1\n");
                fprintf(Jfile,"    isub\n");
                }
            }
        }
    }

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


void  TreeDefine::Codegen_OP_2(MySymbolTable &myTable,TreeNode root,vector<P_element> &OP, vector<E_element> &et,int level)
{
    string node_type=mapping[root->getNodeType()];
    SymbolTable_Element ee=myTable.findSymbol(root->getAttribute()._string);
    P_element PP;
    E_element EE;
    if(node_type=="OP")
    {
        string op_str=mapping[root->getAttribute()._int];
        if(op_str=="PLUS")
        {
            PP.level=level;
            PP.OP = "    iadd";
            OP.push_back(PP);
          //  OP_Type.push_back("    iadd");
        }
        else if(op_str=="MINUS")
        {
            PP.level=level;
            PP.OP = "    isub";
            OP.push_back(PP);
        //    OP_Type.push_back("    isub");
        }
        else if(op_str=="MUL")
        {
            PP.level=level;
            PP.OP = "    imul";
            OP.push_back(PP);
        //    OP_Type.push_back("    imul");
        }
        else if(op_str=="DIV")
        {
            PP.level=level;
            PP.OP = "    idiv";
            OP.push_back(PP);
        //    OP_Type.push_back("    idiv");
        }
    }
    else if(node_type=="CONST")
    {
        string exp_type=mapping[root->getExpType()];
        if(exp_type=="INT")
        {
            int my_Int=static_cast<int>(root->getAttribute()._double);
            string tmp="    ldc "+to_string(my_Int);
            EE.et=tmp;
            EE.level=level;
            et.push_back(EE);
        }
        else if(exp_type=="REAL")
        {
            string tmp="    ldc "+to_string(root->getAttribute()._double);
            EE.et=tmp;
            EE.level=level;
            et.push_back(EE);
        }
    }
    else if(node_type=="PROC_ID")
     {
       //  SymbolTable_Element ee=myTable.findSymbol(root->getAttribute()._string);
         if(ee.K_Type=="variable")
         {
             string type;
             if(ee.type=="int") type="I";
             else if(ee.type=="real") type="F";
             type = "    getstatic "+ProgramName+"/"+ee.name+" "+type;
             EE.et=type;
             EE.level=level;
             et.push_back(EE);
         }
         else if(ee.K_Type=="PARAMETERS")
         {
             string type="    iload_"+to_string(ee.para_index);
             EE.et=type;
             EE.level=level;
             et.push_back(EE);
          //   fprintf(Jfile,"for test para\n");
         }
         else if(ee.K_Type=="FUNCTION")
         {
             string fun_dec=Return_F_Name(ee.name,ee.type);
             fun_dec="    invokestatic "+ProgramName+"/"+fun_dec;
             EE.et=fun_dec;
             EE.level=level;
             et.push_back(EE);
         }
         else if(ee.K_Type=="variable_array")
         {
            string c1=ee.name;
            string c2=ee.type;
            vector<string> t=Spilt_type(c2);
            string type;
            if(t[0]=="int") type="I";
            else if(t[0]=="real") type="F";
            else if(t[0]=="char") type="Ljava/lang/String;";
            string output;
            for(int i=t.size()-1;i>0;i=i-2)
            {
                output=output+"[";
            }
            output="    getstatic "+ProgramName+"/"+c1+" "+output+type+"\n";

            for(auto i = 0u;i<root->getChildren().size();i++)
            {
                if(root->getChildren().at(i)!=NULL)
                {
                    TreeNode c = root->getChildren().at(i);
                    string node_type = mapping[c->getNodeType()];
                    if(node_type=="CONST")
                    {
                        int n1 = static_cast<int>(c->getAttribute()._double);
                        string s1 = "    ldc "+to_string(n1);
                       // fprintf(Jfile,"%s\n",s1.c_str());
                        output = output+s1+"\n";
                      //  fprintf(Jfile,"    ldc 1\n");
                        output+="    ldc 1\n";
                      //  fprintf(Jfile,"    isub\n");
                        output+="    isub\n";
                    }
                    while(c->getSibling()!=NULL)
                    {
                        c=c->getSibling();
                        string node_type2 = mapping[c->getNodeType()];
                        if(node_type2=="CONST")
                        {
                        int n1 = static_cast<int>(c->getAttribute()._double);
                        string s1 = "    ldc "+to_string(n1);
                        // fprintf(Jfile,"    aaload\n");
                        output+="    aaload\n";
                        // fprintf(Jfile,"%s\n",s1.c_str());
                        output = output+s1+"\n";
                        // fprintf(Jfile,"    ldc 1\n");
                        output+="    ldc 1\n";
                        // fprintf(Jfile,"    isub\n");
                        output+="    isub\n";
                        }
                    }
                }
            }
            output+="    iaload";
            //==final==//
            EE.et=output;
            EE.level=level;
            et.push_back(EE);
         }
     }
    else if(node_type=="PROC_FUNC")
    {
        string fun_dec=Return_F_Name(ee.name,ee.type);
        fun_dec="    invokestatic "+ProgramName+"/"+fun_dec;
        EE.et=fun_dec;
        EE.level=level;
        et.push_back(EE);
    }
    for(auto i =0u;i<root->getChildren().size();i++)
    {
         if(mapping[root->getNodeType()]!="PROC_ID"
       //   &&mapping[root->getNodeType()]!="PROC_FUNC"
          &&root->getChildren().at(i)!=NULL)
          {
              Codegen_OP_2(myTable,root->getChildren().at(i),OP,et,level+1);
          }
    }
}
void  TreeDefine::Codegen_OP(MySymbolTable &myTable,TreeNode root,vector<string> &OP_Type, vector<string> &et,string &pp)
 {
     string node_type=mapping[root->getNodeType()];
     SymbolTable_Element ee=myTable.findSymbol(root->getAttribute()._string);
     if(node_type=="OP")
     {
        string op_str=mapping[root->getAttribute()._int];
        if(op_str=="LT")
            OP_Type.push_back("    if_icmpeq ");
        else if(op_str=="LE")
            OP_Type.push_back("    if_icmple ");
        else if(op_str=="GT")
            OP_Type.push_back("    if_icmpgt ");
        else if(op_str=="GE")
            OP_Type.push_back("    if_icmpge ");
        else if(op_str=="EQUAL")
            OP_Type.push_back("    if_icmpeq ");
        else if(op_str=="UNEQUAL")
            OP_Type.push_back("    if_icmpne ");
        else if(op_str=="PLUS")
        {
            OP_Type.push_back("    iadd");
        }
        else if(op_str=="MINUS")
        {
            OP_Type.push_back("    isub");
        }
        else if(op_str=="MUL")
        {
            OP_Type.push_back("    imul");
        }
        else if(op_str=="DIV")
        {
            OP_Type.push_back("    idiv");
        }
        
     }
     else if(node_type=="CONST")
     {
         string exp_type=mapping[root->getExpType()];
         if(exp_type=="INT")
         {
             int my_Int=static_cast<int>(root->getAttribute()._double);
             string tmp="    ldc "+to_string(my_Int);
             et.push_back(tmp);
         }
         else if(exp_type=="REAL")
         {
             string tmp="    ldc "+to_string(root->getAttribute()._double);
             et.push_back(tmp);
         }
     }
     else if(node_type=="PROC_ID")
     {
       //  SymbolTable_Element ee=myTable.findSymbol(root->getAttribute()._string);
         if(ee.K_Type=="variable")
         {
             string type;
             if(ee.type=="int") type="I";
             else if(ee.type=="real") type="F";
             type = "    getstatic "+ProgramName+"/"+ee.name+" "+type;
             et.push_back(type);
         }
         else if(ee.K_Type=="PARAMETERS")
         {
             string type="    iload_"+to_string(ee.para_index);
             et.push_back(type);
          //   fprintf(Jfile,"for test para\n");
         }
         else if(ee.K_Type=="FUNCTION")
         {
             string fun_dec=Return_F_Name(ee.name,ee.type);
             fun_dec="    invokestatic "+ProgramName+"/"+fun_dec;
             et.push_back(fun_dec);
         }
     }
     else if(node_type=="PROC_FUNC")
     {
        string fun_dec=Return_F_Name(ee.name,ee.type);
        fun_dec="    invokestatic "+ProgramName+"/"+fun_dec;
        OP_Type.push_back(fun_dec);
     }
     for(auto i =0u;i<root->getChildren().size();i++)
     {
         if(mapping[root->getNodeType()]!="PROC_ID"
       //   &&mapping[root->getNodeType()]!="PROC_FUNC"
          &&root->getChildren().at(i)!=NULL)
          {
              Codegen_OP(myTable,root->getChildren().at(i),OP_Type,et,pp);
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
        else if(mapping[ch3->getNodeType()]=="PROC_FUNC"&&myTable.bFind(ch3->getAttribute()._string,0,ch3->getLineNumber(),ch3->getColNumber()))
        {
            string f_name = ch3->getAttribute()._string;
            SymbolTable_Element ee = myTable.findSymbol(f_name);
            vector<string> tt = Spilt_type(ee.type);
            check_para.push_back(tt[0]);
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
    
    //紀錄返回值類型 並給出相應的return形式(codegen)
    int return_number=0;

    if(mapping[root->getChildren().at(0)->getNodeType()]=="PROCEDUREHEAD"
    || mapping[root->getChildren().at(0)->getNodeType()]=="FUNCTIONHEAD")
    { 
         my_ID = root->getChildren().at(0)->getAttribute()._string;
         row   = root->getChildren().at(0)->getLineNumber();
         col   = root->getChildren().at(0)->getColNumber();
    }
    tmp_node = root->getChildren().at(0);

    //=====函數返回值=====//
    if(tmp_node->getChildren().size()==1)
     {  
         node_type +="void";
     }
    
    else if(mapping[tmp_node->getChildren().at(1)->getExpType()]=="INT")
    {
        node_type +="int";
        return_number=1;
    }
    else if(mapping[tmp_node->getChildren().at(1)->getExpType()]=="REAL")
    {
        node_type +="real";
        return_number=2;
    }
    else if(mapping[tmp_node->getChildren().at(1)->getExpType()]=="STRING")
    {
        node_type +="string";
        return_number=3;
    }

    //=====函數參數=====//
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

        //====codegen====//
        string fun_decl =Return_F_Name( element.name,element.type);
        fun_decl = ".method public static "+fun_decl;
        fprintf(Jfile,"%s\n",fun_decl.c_str());
        fun_decl=".limit locals 50\n.limit stack 50\n";
        fprintf(Jfile,"%s",fun_decl.c_str());
        //===============//

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
        element.K_Type = "PARAMETERS";
        element.para_index = static_cast<int>(i);
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
                Sub_Var_decl(subTable,tmp_node->getChildren().at(i),scope,subShow);
                TreeNode           t1=tmp_node->getChildren().at(i);
                while(t1->getSibling()!=NULL)
                {
                    Sub_Var_decl(subTable,t1->getSibling(),scope,subShow);
                    t1=t1->getSibling();
                }
            }
            //handle LABEL of Function/Procedure
            
            else if(mapping[tmp_node->getChildren().at(i)->getNodeType()]=="LABEL")
            {
               New_Label_Check(subTable,tmp_node->getChildren().at(i),suc_defunc,row,col,0,my_ID);
                TreeNode t2=tmp_node->getChildren().at(i);
                while(t2->getSibling()!=NULL)
                {
                    if(mapping[t2->getSibling()->getNodeType()]=="LABEL")
                       New_Label_Check(subTable,t2->getSibling(),suc_defunc,row,col,0,my_ID);
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
 //====codegen=====
 fprintf(Jfile,"Lf:\n");
 if(return_number==0)
    fprintf(Jfile,"    return\n");
 else if(return_number==1)
    fprintf(Jfile,"    ireturn\n");
 else if(return_number==2)
    fprintf(Jfile,"    freturn\n");

 fprintf(Jfile,".end method\n");
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
   //=====codegen=====//
   string var_sts=""; 

   //type check and insert symbol table
   TreeNode tmp_2 = root->getChildren().at(1);
   string exp_type = mapping[tmp_2->getExpType()];
   if(exp_type!="ARRAY")
   {
        if(exp_type=="INT") 
        {
            exp_type="int";
            var_sts = " I";
        }
        else if(exp_type=="REAL") 
        {
            exp_type="real";
            var_sts = " F";
        }
        else if(exp_type=="STRING") 
        {
            exp_type="string";
            var_sts = " S";
        }
        else if(exp_type=="CHAR") 
        {
            exp_type="char";
            var_sts = " Ljava/lang/String;";
        }
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

                string tmp_3 = ".field public static ";
                tmp_3 =tmp_3+element.name+var_sts+"\n";
                if(element.scope==0)
                    fprintf(Jfile,"%s",tmp_3.c_str());

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
   if(exp_type=="INT") 
   {
       exp_type="int";
       var_sts = "I";
   }
   else if(exp_type=="REAL")
   {
       exp_type="real";
       var_sts = "F";
   }
   else if(exp_type=="STRING")
   {
        exp_type="string";
        var_sts = "S";
   }
   else if(exp_type=="CHAR")
   {
       exp_type="char";
       var_sts ="Ljava/lang/String;";
   }

   string stmp=exp_type;

   for(int i=array_index.size()-1;i>=0;i-=2)
   {
    stmp+="[";
    stmp+=to_string(array_index[i-1]);
    stmp+="~";
    stmp+=to_string(array_index[i]);
    stmp+="]";
    var_sts = "["+var_sts;
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

           //====for codegen====//
           string tmp_3 = ".field public static ";
           tmp_3 =tmp_3+element.name+" "+var_sts+"\n";
           if(element.scope==0)
            fprintf(Jfile,"%s",tmp_3.c_str());
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

