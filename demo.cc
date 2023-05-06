//Brandon Fenske
//CSE340 - Project 3
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h> 
#include <ctype.h> 
#include <string.h> 
#include "compiler.h"
#include "lexer.h"
#include <iostream>
#include <unordered_map>

using namespace std;

//function names, added .h to .cc,added namespace std
class CFG{

public:
    // Grammar Rules

    struct InstructionNode* Case_CFG(struct InstructionNode*); 
    struct InstructionNode* Case_List_CFG(struct InstructionNode*);
    struct InstructionNode* For_Stmnt_CFG(struct InstructionNode*);
    struct InstructionNode* Body_CFG(); 
    struct InstructionNode* Default_Case_CFG();
    struct InstructionNode* Program_CFG(); 
    struct InstructionNode* Stmnt_List_CFG(); 
    struct InstructionNode* Switch_Stmnt_CFG();
    struct InstructionNode* Stmnt_CFG(); 

    TokenType Op_CFG();
    TokenType Relop_CFG();
        
    void Assign_Stmnt_CFG(struct InstructionNode*); 
    void Condition_CFG(struct InstructionNode*);
    void Expr_CFG(struct InstructionNode*); 
    void If_Stmnt_CFG(struct InstructionNode*);
    void Input_Stmnt_CFG(struct InstructionNode*);
    void Output_Stmnt_CFG(struct InstructionNode*); 
    void While_Stmnt_CFG(struct InstructionNode*);
    void Num_List_CFG();
    void Id_List_CFG(); 
    void Inputs_CFG();
    int Primary_CFG(); 
    void Var_Section_CFG(); 
    int IndexedAt(string);

    void syntaxError(); 
    private:
    Token currentToken(TokenType expected_types); // checks if the token is the currentToken type
    string tokenTypeToString(TokenType type); //converts token type to string

};

LexicalAnalyzer myLexer; // lexer object
struct InstructionNode * first; // start node for the CFG

unordered_map< string,int > CFGMAP; // hash map for the CFG

int currentIndex=0; // current index of the token vector

void CFG:: syntaxError(){ //prints out syntax error and exits

    cout << "Syntax Error" << endl;
    exit(1);

}
int CFG::IndexedAt(string mystring) {
    //cout << " in location\n";
    if (CFGMAP.find(mystring) == CFGMAP.end()) {
        // Variable not found in map, create a new entry
        int address = next_available;
        mem[next_available] = (isdigit(mystring[0])) ? std::stoi(mystring) : 0;
        next_available++;
        CFGMAP.insert(make_pair(mystring, address));
    }
    return CFGMAP.at(mystring);
}

Token CFG::currentToken(TokenType expected_type)
{
    Token myToken = myLexer.GetToken();

    //cout << "Current token type: " << tokenTypeToString(myToken.token_type) << endl;

    if (myToken.token_type != expected_type) {
        //cout << "Expected token type: " << tokenTypeToString(expected_type) << endl;
        //cout << "Error: unexpected token type!" << endl;
    }

    return myToken;
}

string CFG::tokenTypeToString(TokenType type)
{
    switch (type) {
        case TokenType::SWITCH:
            return "SWITCH";
        case TokenType::CASE:
            return "CASE";
        case TokenType::COLON:
            return "COLON";
        case TokenType::DEFAULT:
            return "DEFAULT";
        case TokenType::PLUS:
            return "PLUS";
        case TokenType::MINUS:
            return "MINUS";
        case TokenType::ID:
            return "ID";
        case TokenType::NUM:
            return "NUM";
        case TokenType::INPUT:
            return "INPUT";
        case TokenType::OUTPUT:
            return "OUTPUT";
        case TokenType::SEMICOLON:
            return "SEMICOLON";
        case TokenType::LPAREN:
            return "LPAREN";
        case TokenType::RPAREN:
            return "RPAREN";
        case TokenType::LBRACE:
            return "LBRACE";
        case TokenType::RBRACE:
            return "RBRACE";
        case TokenType::COMMA:
            return "COMMA";
        case TokenType::EQUAL:
            return "EQUAL";
        case TokenType::NOTEQUAL:
            return "NOTEQUAL";
        case TokenType::GREATER:
            return "GREATER";
        case TokenType::VAR:
            return "VAR";
        case TokenType::WHILE:
            return "WHILE";
        case TokenType::FOR:
            return "FOR";
        case TokenType::END_OF_FILE:
            return "END_OF_FILE";
        case TokenType::LESS:
            return "LESS";
        default:
            return "UNKNOWN";
    }
}
struct InstructionNode* parse_generate_intermediate_representation()//main function from compiler
{
    //cout << " in parse_generate\n";
    CFG myCFG;

    return myCFG.Program_CFG();
}


struct InstructionNode * CFG:: Program_CFG() //first rule of the CFG
{
    //cout << " in program \n";
    Var_Section_CFG(); //first option
    first =  Body_CFG(); //second option
    Inputs_CFG(); //third option
    currentToken(END_OF_FILE); // else end of file

    return first;
}
void CFG:: Var_Section_CFG(){ //from Program_CFG
    //cout << " in var section \n";
    Id_List_CFG();
    Token MyToken;
    currentToken(SEMICOLON);
    MyToken = myLexer.peek(1);
    if(MyToken.token_type == ID){
        Var_Section_CFG();
    }
    else{
        return;
    }
}
void CFG:: Id_List_CFG() //Id_List_CFG from Var_Section_CFG
{
    //cout << " in id list \n";
    string myIndex;
    Token myToken;
    myIndex = currentToken(ID).lexeme;

    int pointerAdr =next_available;
    mem[next_available] = 0;
    next_available = next_available + 1;

    CFGMAP.insert(make_pair(myIndex, pointerAdr));
    myToken = myLexer.peek(1);
    
    switch(myToken.token_type) {
        case COMMA:
            currentToken(COMMA);
            Id_List_CFG(); //if theres a comma we go back to Id_List_CFG
            break;
        case SEMICOLON: //semicolon means we are done
            return;
            break;
        default:
            syntaxError();
            break;
}}

struct InstructionNode* CFG:: Body_CFG() //Body_CFG from Program_CFG
{
    //cout << " in body \n";
    struct InstructionNode* stmtList; 
    
    currentToken(LBRACE); 
    stmtList =  Stmnt_List_CFG(); 
    currentToken(RBRACE); 

    return stmtList;
}

struct InstructionNode* CFG:: Stmnt_List_CFG() //Stmnt_List_CFG from Body_CFG
{   
    //cout << " in stmt list \n";
    struct InstructionNode* myStmtList; //create stmtList
    myStmtList=Stmnt_CFG();

    struct InstructionNode* AppendStmtList; //second stmtList for appending if needed

    Token myToken =myLexer.peek(1);
    if(myToken.token_type == RBRACE) //if theres a right brace we are done
    {
        return myStmtList;
    }
    else
    {
        struct InstructionNode* index = myStmtList;
        AppendStmtList =Stmnt_List_CFG();

            while (index-> next != NULL){ //add next index to stmtList while not null
                index = index-> next;
            }
        index-> next = AppendStmtList; //append the stmtList

        return myStmtList;
}}
// struct InstructionNode* CFG:: Stmnt_CFG() //from Stmnt_List_CFG
// { ...
// else if (myToken.token_type == FOR) { //For Statement CFG
//     cout << " in FOR in SWITCH TOKEN statement \n";
//     struct InstructionNode *forNode = new InstructionNode;
//     struct InstructionNode *for_Stmnt = For_Stmnt_CFG(forNode); //call the for statement
//     struct InstructionNode *index = forNode;
//     while (index->next != NULL) {
//         index = index->next;
//     }

//     index->next = for_Stmnt; //set the next index to the for statement
//     cout << " exiting FOR in SWITCH TOKEN statement \n";
//     return forNode;
// }
// ...}

struct InstructionNode* CFG:: Stmnt_CFG() //from Stmnt_List_CFG
{
   // cout << " in stmt  \n";
    struct InstructionNode* instructionNode = new InstructionNode; //create new instruction node
    Token myToken = myLexer.peek(1); //peek at the next token

    instructionNode-> next = NULL; //set next to null

    if(myToken.token_type == ID)
    {
        Assign_Stmnt_CFG(instructionNode); //if its an ID we go to Assign_Stmnt_CFG
        return instructionNode;
    }

    else if(myToken.token_type == WHILE)
    {

        While_Stmnt_CFG(instructionNode); //if its a while we call While_Stmnt_CFG

        struct InstructionNode* myIndex = instructionNode;
        while(myIndex-> next != NULL)
        {
            myIndex = myIndex-> next;
        }

        struct InstructionNode* noOp = new InstructionNode; //no operation
        noOp-> type = NOOP;
        noOp-> next = NULL;

        struct InstructionNode* MyJmp = new InstructionNode; //jump instance
        MyJmp-> type = JMP;
        MyJmp-> jmp_inst.target = instructionNode;


        myIndex-> next = MyJmp; //set the next index to the jump
        MyJmp-> next = noOp; //set the jump to the no operation


        instructionNode-> cjmp_inst.target = noOp; //set the target to the no operation

        return instructionNode; //return the instruction node
    }
    else if(myToken.token_type == IF) //if the token is an if 
    {
        struct InstructionNode* noOp = new InstructionNode; //set up the no operation
        struct InstructionNode* index = instructionNode; //set up the index
        noOp-> next = NULL; //set next to null
        noOp-> type = NOOP; //set the type to no operation
        

        If_Stmnt_CFG(instructionNode); //call the if statement


        while(index-> next != NULL)//while the index is not null we go to the next index
        { 
            index = index-> next;
        }   

        index-> next = noOp;
        instructionNode-> cjmp_inst.target = noOp; //set the target to the no operation

        return instructionNode;
    }
    else if(myToken.token_type == SWITCH)//if the token is a switch we call Switch_Stmnt_CFG
    { 
        instructionNode = Switch_Stmnt_CFG();
        return instructionNode;

    }
    else if (myToken.token_type == FOR) { //For Statement CFG
   // cout << " in FOR in SWITCH TOKEN statement \n";
    struct InstructionNode *JUMP = new InstructionNode;
    struct InstructionNode *noOp = new InstructionNode;
    struct InstructionNode *for_Stmnt = For_Stmnt_CFG(instructionNode); //call the for statement
    JUMP->type = JMP;
    noOp->type = NOOP;
    noOp->next = NULL;

    JUMP->jmp_inst.target = instructionNode->next;
    
    struct InstructionNode *index = instructionNode;
    while (index->next != NULL) {
        index = index->next;
    }
    index->next = for_Stmnt;
    index = index->next;
    index->next = JUMP;
    JUMP->next = noOp;

    instructionNode->next->cjmp_inst.target = noOp;
    //cout << " exiting FOR in SWITCH TOKEN statement \n";
    return instructionNode;
}

    else if(myToken.token_type ==OUTPUT) //if the token is an output we call Output_Stmnt_CFG
    {
        Output_Stmnt_CFG(instructionNode);

        return instructionNode;
    }
    else if(myToken.token_type ==INPUT) //if the token is an input we call Input_Stmnt_CFG
    {
        Input_Stmnt_CFG(instructionNode);

        return instructionNode;
    }
    else
    {
        syntaxError();
}}

void CFG:: Assign_Stmnt_CFG(struct InstructionNode* assnStm) //from Stmnt_CFG
{
    //cout << " in assnmt stmt \n";
    Token Current = currentToken(ID); //get the current token

    assnStm-> type = ASSIGN; //set the type to assign
    assnStm-> assign_inst.left_hand_side_index = IndexedAt(Current.lexeme); //EQUAL
    currentToken(EQUAL); //get the next token

        //we will either have a primary or an expression
    Token myToken = myLexer.peek(2);

    if(myToken.token_type ==SEMICOLON) { //if the token is a semicolon we set the operator to none
        assnStm-> assign_inst.op = OPERATOR_NONE;
        assnStm-> assign_inst.operand1_index =  Primary_CFG();

        currentToken(SEMICOLON);
    }
    else if(myToken.token_type == PLUS || myToken.token_type == MINUS ||  //if the token is an operator we call Expr_CFG
            myToken.token_type == MULT || myToken.token_type == DIV){

        Expr_CFG(assnStm);
        currentToken(SEMICOLON);
    }
    else{
        syntaxError(); //otherwise we have a syntax error
}}

void CFG :: Expr_CFG(struct InstructionNode* InstNode) //from Assign_Stmnt_CFG
{
    //cout << " in expr cfg \n";
    InstNode-> assign_inst.operand1_index =  Primary_CFG(); //set the operand to the primary

    TokenType Current = Op_CFG(); //get the operator
    //Operator is either plus, minus, mult, or div

    switch(Current) {
        case PLUS:
            InstNode-> assign_inst.op = OPERATOR_PLUS;
            break;
        case MINUS:
            InstNode-> assign_inst.op = OPERATOR_MINUS;
            break;
        case MULT:
            InstNode-> assign_inst.op = OPERATOR_MULT;
            break;
        case DIV:
            InstNode-> assign_inst.op = OPERATOR_DIV;
            break;
        default:
            syntaxError();
            break;
    }

    //then we get the next primary as per CFG
    InstNode-> assign_inst.operand2_index =  Primary_CFG();
}

int CFG:: Primary_CFG() //from Expr_CFG
{
    //cout << " in primary                 (NUM | INT)  \n";
    Token myToken = myLexer.peek(1); //peek next token
    //Rules for primary either ID or NUM
    if(myToken.token_type == ID)
    {
        return IndexedAt(currentToken(ID).lexeme); //return the index of the id
    }
    else if(myToken.token_type == NUM)
    {
        string MyNumber;
        MyNumber =currentToken(NUM).lexeme;//get the number

        if( CFGMAP.find(MyNumber) != CFGMAP.end()){ //if the number is in the map
            return CFGMAP.at(MyNumber); //return the index
        }else{
            //add to the map
            int insertToMap = next_available; 
            mem[next_available] = stoi(MyNumber); //convert the string to an int
            next_available = next_available + 1;
            CFGMAP.insert(make_pair(MyNumber, insertToMap)); //insert the number into the map

            return CFGMAP.at(MyNumber); //return the index
        }}else{
            syntaxError();
            }
}
TokenType CFG:: Op_CFG() //from Expr_CFG
{
    //cout << " in op  \n";
    Token myToken = myLexer.peek(1); //again, we peek

    switch(myToken.token_type) { //swtich based on the token type
        case PLUS:
            return currentToken(PLUS).token_type;
        case MINUS:
            return currentToken(MINUS).token_type;
        case MULT:
            return currentToken(MULT).token_type;
        case DIV:
            return currentToken(DIV).token_type;
        default:
            syntaxError();
            break;
}}

void CFG:: If_Stmnt_CFG(struct InstructionNode* InstNode)//from Stmnt_CFG
{
    //cout << " in if stmt \n";
    InstNode-> type = CJMP;
    currentToken(IF);//IF

    Condition_CFG(InstNode); //then we call condition
    InstNode-> next =  Body_CFG(); //then we call body
}


void CFG:: Input_Stmnt_CFG(struct InstructionNode* InstNode)//from Stmnt_CFG
{
    //cout << " in input stmt \n";
    InstNode-> type = IN;
    currentToken(INPUT); //input is first rule

     InstNode-> input_inst.var_index = IndexedAt(currentToken(ID).lexeme); //get the index of the id
    currentToken(SEMICOLON); //then semicolon
}
void CFG:: Output_Stmnt_CFG(struct InstructionNode* InstNode)//from Stmnt_CFG
{
    //cout << " in output stmt  \n";
    InstNode-> type = OUT;
    currentToken(OUTPUT); //output is first rule

    InstNode-> output_inst.var_index = IndexedAt(currentToken(ID).lexeme); //get the index of the id
    currentToken(SEMICOLON); //then semicolon
}


void CFG:: While_Stmnt_CFG(InstructionNode* InstNode)//from Stmnt_CFGw
{
    //cout << " in while stmt \n";
    InstNode-> type = CJMP;
    currentToken(WHILE); //while is first rule

    Condition_CFG(InstNode);
    InstNode-> next =  Body_CFG(); //then we call body
}


void CFG:: Condition_CFG(struct InstructionNode* InstNode)//from If_Stmnt_CFG or For_Stmnt_CFG or While_Stmnt_CFG
{
    //cout << " in condition               (primary | relop | primary) \n";
    InstNode-> cjmp_inst.operand1_index =  Primary_CFG(); //first operand is primary

    TokenType myToken =  Relop_CFG(); //token is either greater, less, or notequal

    //we switch based on the token type and set the condition op
    switch(myToken) {
        case GREATER:
            InstNode-> cjmp_inst.condition_op = CONDITION_GREATER;
            break;
        case LESS:
            InstNode-> cjmp_inst.condition_op = CONDITION_LESS;
            break;
        case NOTEQUAL:
            InstNode-> cjmp_inst.condition_op = CONDITION_NOTEQUAL;
            break;
        default:
            syntaxError();
            break;
    }
    InstNode-> cjmp_inst.operand2_index =  Primary_CFG();
}


TokenType CFG::Relop_CFG() //from Condition_CFG
{
    //cout << " in relop              (greater | less | equal)\n";
    //Greater, Less, or Not Equal
    Token myToken = myLexer.peek(1);
    
    switch (myToken.token_type) {
        case GREATER:
            return currentToken(GREATER).token_type;
        case LESS:
            return currentToken(LESS).token_type;
        case NOTEQUAL:
            return currentToken(NOTEQUAL).token_type;
        default:
            syntaxError();
            break;
}}

struct InstructionNode* CFG::Switch_Stmnt_CFG() {
    currentToken(SWITCH);
    currentIndex = IndexedAt(currentToken(ID).lexeme);
    currentToken(LBRACE);

    InstructionNode* label = new InstructionNode;
    label->type = NOOP;
    label->next = NULL;

    InstructionNode* NewNode = Case_List_CFG(label);

    Token myToken = myLexer.peek(1);
    if (myToken.token_type == DEFAULT) {
        struct InstructionNode* def = Default_Case_CFG();
        struct InstructionNode* current = NewNode;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = def;
    }

    currentToken(RBRACE);
    struct InstructionNode* current = NewNode;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = label;

    return NewNode;
}


InstructionNode* CFG::For_Stmnt_CFG(InstructionNode* InstNode) //from Stmnt_CFG
{
    //cout << "in For_Stmnt_CFG";
    
    currentToken(FOR);
    currentToken(LPAREN);

    Assign_Stmnt_CFG(InstNode); // initial assignment

    struct InstructionNode* condition = new InstructionNode;
    Condition_CFG(condition); // condition
    condition->type = CJMP;
    InstNode->next = condition; // link initial assignment to condition

    currentToken(SEMICOLON);

    struct InstructionNode* assign = new InstructionNode;
    Assign_Stmnt_CFG(assign); // loop update assignment

    currentToken(RPAREN);

    condition->next = Body_CFG(); // link condition to loop body

    return assign;
}



InstructionNode* CFG::Case_List_CFG(struct InstructionNode* variable) {
    //cout << " in case list \n";
    struct InstructionNode* Case1;
    struct InstructionNode* Case2;
    
    Case1 = Case_CFG(variable); //case with only one call

    Token myToken = myLexer.peek(1);

    if (myToken.token_type == RBRACE || myToken.token_type == DEFAULT) {
        return Case1;
    }
    else if (myToken.token_type == CASE) {
        struct InstructionNode* current = Case1;
        Case2 = Case_List_CFG(variable);      
        while (current->next != NULL){ 
            current = current->next;
        }
        current->next = Case2; //and set the next to case 2
        return Case1;
    }
}

struct InstructionNode* CFG::Case_CFG(struct InstructionNode* label) {
    //cout << "in case \n";
    currentToken(CASE);
    int myoperator;
    string number;
    //cout << "in case again";
    number = currentToken(NUM).lexeme;
    
    if (CFGMAP.find(number) == CFGMAP.end()) {
        int address = next_available;
        mem[next_available] = std::stoi(number);
        next_available++;
        CFGMAP.insert(std::make_pair(number, address));
    }
    
    myoperator = CFGMAP.at(number);
    currentToken(COLON);
    
    struct InstructionNode* newNode = new InstructionNode;
    newNode->type = CJMP;
    newNode->next = NULL;
    newNode->cjmp_inst.condition_op = CONDITION_NOTEQUAL;
    newNode->cjmp_inst.operand1_index = currentIndex;
    newNode->cjmp_inst.operand2_index = myoperator;
    newNode->cjmp_inst.target = Body_CFG();

    struct InstructionNode* gotoLabel = new InstructionNode;
    gotoLabel->type = JMP;
    gotoLabel->next = NULL;
    gotoLabel->jmp_inst.target = label;

    struct InstructionNode* current = newNode->cjmp_inst.target;
    while(current->next != NULL){
        current = current->next;
    } 
    current->next = gotoLabel;

    return newNode;
}


struct InstructionNode* CFG:: Default_Case_CFG()// default 
{
    //cout << " in default case \n";
    struct InstructionNode* InstNode = new InstructionNode;

    //few rules for default case just default colon and call body
    currentToken(DEFAULT);
    currentToken(COLON);

    InstNode =  Body_CFG();

    return InstNode;
}

void CFG:: Inputs_CFG(){ //just call num_list
    //cout << " in inputs \n";
    Num_List_CFG();
}

void CFG:: Num_List_CFG() //from inputs
{
    //cout << " in num list \n";
    int stringInt;
    string myInput;

    myInput = currentToken(NUM).lexeme; //get the number and set it to myinput

    stringInt = stoi(myInput); //convert the string to an int
    inputs.push_back(stringInt); //push it back to the vector

    switch(myLexer.peek(1).token_type) {
        case NUM://if the next token is a number then we call num list again
            Num_List_CFG();
            break;
        case END_OF_FILE://otherwise we are done if eof
            return;
            break;
        default:
            syntaxError();
            break;
    }
}
