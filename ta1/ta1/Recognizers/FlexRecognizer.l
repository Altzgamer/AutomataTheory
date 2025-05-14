%option noyywrap
%option never-interactive
%option nounistd

%{
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "IMyRecognizer.h"
#include "FlexRecognizer.h"

static std::string commandName;
static std::vector<std::string> tempKeys;
static bool commandValid = false;

%}

COMMAND    [a-zA-Z0-9./]+
SHORT_KEY  -[a-zA-Z0-9]
LONG_KEY   --[a-zA-Z0-9]+

%s KEYS EXPECT_SPACE

%%

^({SHORT_KEY}|{LONG_KEY}) {
    commandValid = false;
    BEGIN(INITIAL);
}

{COMMAND} {
    commandName = yytext;
    tempKeys.clear();
    commandValid = true;
    BEGIN(KEYS);
}

<KEYS>{SHORT_KEY} {
    tempKeys.push_back(yytext);
    BEGIN(EXPECT_SPACE);
}

<KEYS>{LONG_KEY} {
    tempKeys.push_back(yytext);
    BEGIN(EXPECT_SPACE);
}

<KEYS>[ \t]+ {}

<KEYS>(\n|$) {
    BEGIN(INITIAL);
}

<EXPECT_SPACE>[ \t]+ {
    BEGIN(KEYS);
}

<EXPECT_SPACE>(\n|$) {
    BEGIN(INITIAL);
}

<EXPECT_SPACE>. {
    commandValid = false;
    BEGIN(INITIAL);
}

<KEYS>. {
    commandValid = false;
    BEGIN(INITIAL);
}

<<EOF>> {
    return 0;
}

%%

bool FlexRecognizer::processCommand(const std::string& input, std::unordered_map<std::string, std::vector<std::string>>& commandKeys) const {
    commandValid = false;
    commandName.clear();
    tempKeys.clear();

    YY_BUFFER_STATE buffer = yy_scan_string(input.c_str());
    yylex();
    
    yy_delete_buffer(buffer);
    yyrestart(yyin);
	

    if (commandValid && !commandName.empty()) {
        auto& keys = commandKeys[commandName];
        keys.insert(keys.end(), tempKeys.begin(), tempKeys.end());
    }

    return commandValid;
}