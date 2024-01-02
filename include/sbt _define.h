#ifndef __SBT_DEFINE_H__
#define __SBT_DEFINE_H__
#include "ast.h"
#include "info.h"

extern int line_no, col_no, yyleng;
extern bool function_noreturn_show;
struct SymbolTable_Element
{
  string name;
  int scope;
  string type;
  string K_Type;
  bool bDecl=false;
  int col;
  int row;
};

struct para_element
{
   string para_id;
   string para_TYPE;
};



class MySymbolTable
{
private:
   MySymbolTable* parentSymbolTable=NULL; 
   
public:
   unordered_map<string,SymbolTable_Element> symbolMap;


   bool bHasParent()
   {
      if(parentSymbolTable!=NULL)
         return true;
      else 
         return false;
   }

   void Setparent(MySymbolTable *parent)
   {
      if(parent != nullptr)
         parentSymbolTable = parent;
      else
         cout<<"Error: Trying to set a null parent symbol table"<<endl;
   }

   //Add something to symboltalbe
   bool addSymbol(const string &key,const SymbolTable_Element &value)
   {
      auto it = symbolMap.find(key);
      string tmpk=key;

      if(it!=symbolMap.end())
       {
      if(value.K_Type=="variable")
   	   fprintf(stderr,REDEF_VAR,value.row, value.col, tmpk.c_str());
      else if(value.K_Type=="variable_array")
   	   fprintf(stderr,REDEF_VAR,value.row, value.col, tmpk.c_str());
      else if(value.K_Type=="FUNCTION")
      { 
        // cout<<"33333"<<endl;
         function_noreturn_show = false;
         fprintf(stderr,RETURN_VAL,value.row, value.col, tmpk.c_str());
         fprintf(stderr,REDEF_FUN,value.row, value.col, tmpk.c_str());
      }
      return false;
       }
      else
      {
         symbolMap[key]=value;
        return true;
      }
   }

   //find something from symboltable
   SymbolTable_Element findSymbol(const string &key)
   {
      auto it = symbolMap.find(key);
      if(it !=symbolMap.end())
      {
         return it->second;
      }
      else
      {
         if(this->bHasParent())
         {
          return  this->parentSymbolTable->findSymbol(key);
         }
         else
         {
            cout<<"Key not found,key="<<key<<endl;
         }
         return SymbolTable_Element();
      }
   }

   bool bFind(const string &key,int check,int row,int col)
   {
      auto it = symbolMap.find(key);
      if(it !=symbolMap.end())
      {
         return true;
      }
      else
      {
         //check: function symboltable-->global symboltable
         if(this->bHasParent())
         {
          //  Find_parent++;
            return this->parentSymbolTable->bFind(key,check,row,col);
         }
         else
         { 
               string tmp=key;
               if(check==0)
               {
                  fprintf(stderr,UNDEC_VAR,row, col, tmp.c_str());
               }
               else if(check==1)
               {
             //cout<<"key="<<key<<endl;
                  fprintf(stderr,UNDEC_FUN,row, col, tmp.c_str());
               }

               return false;
         }
         return false;
         
      }
   }

    bool New_bFind(const string &key,int type,int row,int col)
   {
    /*  auto it = symbolMap.find(key);
      if(it !=symbolMap.end())
      {
         return true;
      }
      else
      {
         //check: function symboltable-->global symboltable
         if(this->bHasParent())
         {
            return this->parentSymbolTable->New_bFind(key,type,row,col);
         }
         else
         {
         string tmp=key;
         if(type==0)
             fprintf(stderr,UNDEC_VAR,row, col, tmp.c_str());
         else if(type==1)
             fprintf(stderr,UNDEC_FUN,row, col, tmp.c_str());
         }
         return false;
        
      }*/
      return false;
   }
   //remove something from symboltable
   void removeElementsByScope(int targetScope)
   {
      for(auto it = symbolMap.begin();it!=symbolMap.end();)
      {
         if(it->second.scope == targetScope)
         {
            it = symbolMap.erase(it);
         }
         else
         {
            ++it;
         }
      }
   }

   void clear()
   {
      symbolMap.clear();
   }
};

//static vector<MySymbolTable> symbolTable_K;
static MySymbolTable symbolTable_Q;
#endif
