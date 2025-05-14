#include <iostream>
#include <string>
#include <memory>
#include "../ast/ast.h"
#include "../ast_builder/ast_builder.h"
#include "../nfa/nfa.h"
#include "../dfa/dfa.h"
#include "../my_regex/my_regex.h"
#include <gtest/gtest.h>

using namespace regex;

TEST(check_build, Subtest_1)
{
        std::string s = "a";

	Regex rv(s);
	ASSERT_TRUE(rv.compile(1) == 0);
}

TEST(check_build, Subtest_2)
{
        std::string s = "aa";

	Regex rv(s);
	ASSERT_TRUE(rv.compile(2) == 0);
}

TEST(check_build, Subtest_3)
{
        std::string s = "ab";

	Regex rv(s);
	ASSERT_TRUE(rv.compile(3) == 0);
}

TEST(check_build, Subtest_4)
{
        std::string s = "a|b";

	Regex rv(s);
	ASSERT_TRUE(rv.compile(4) == 0);
}

TEST(check_build, Subtest_5)
{
        std::string s = "a+";

	Regex rv(s);
	ASSERT_TRUE(rv.compile(5) == 0);
}

TEST(check_build, Subtest_6)
{
        std::string s = "ab+";

	Regex rv(s);
	ASSERT_TRUE(rv.compile(6) == 0);
}

TEST(check_build, Subtest_7)
{
        std::string s = "a+b|aaab";

	Regex rv(s);
	ASSERT_TRUE(rv.compile(7) == 0);
}


TEST(check_build, Subtest_8)
{
        std::string s = "(a|b|c)+";

	Regex rv(s);
	ASSERT_TRUE(rv.compile(8) == 0);
}


TEST(check_build, Subtest_9)
{
        std::string s = "me{2}|p(hi)";

	Regex rv(s);
	ASSERT_TRUE(rv.compile(9) == 0);
}

TEST(check_build, Subtest_10)
{
        std::string s = "a(bc|de{2}){2}";

	Regex rv(s);
	ASSERT_TRUE(rv.compile(10) == 0);
}

TEST(check_build, Subtest_11)
{
        std::string s = "(xy|z){1,3}w";

	Regex rv(s);
	ASSERT_TRUE(rv.compile(11) == 0);
}

TEST(check_find_all, Subtest_1)
{
    std::vector<std::string> checking = {
        "abc",
        "abbc",
        "d",
        "abcdef"
    };
    std::string s = "a+b+c+"; 

    Regex rv(s);
    ASSERT_EQ(rv.compile(12), 0);

    std::vector<std::string> ans;
    rv.find_all(checking, ans);

    std::vector<std::string> expected = {"abc", "abbc", "abc"};
    ASSERT_EQ(ans.size(), expected.size());
    for (size_t k = 0; k < expected.size(); k++)
        EXPECT_EQ(ans[k], expected[k]);
}

TEST(check_find_all, Union)
{
    std::vector<std::string> checking = {
        "abc",
        "bcd",
        "cab"
    };
    std::string s = "a|b";

    Regex rv(s);
    ASSERT_EQ(rv.compile(13), 0);

    std::vector<std::string> ans;
    rv.find_all(checking, ans);


    std::vector<std::string> expected = {"a","b","b","a","b"};
    ASSERT_EQ(ans.size(), expected.size());
    for (size_t k = 0; k < expected.size(); ++k)
        EXPECT_EQ(ans[k], expected[k]);
}

// positive closure +
TEST(check_find_all, PositiveClosure)
{
    std::vector<std::string> checking = {
        "ababx",
        "zab",
        "xx"
    };
    std::string s = "(ab)+";

    Regex rv(s);
    ASSERT_EQ(rv.compile(14), 0);

    std::vector<std::string> ans;
    rv.find_all(checking, ans);


    std::vector<std::string> expected = {"abab","ab"};
    ASSERT_EQ(ans.size(), expected.size());
    for (size_t k = 0; k < expected.size(); ++k)
        EXPECT_EQ(ans[k], expected[k]);
}

// fixed-range {2,3}
TEST(check_find_all, Range2to3)
{
    std::vector<std::string> checking = {
        "aaaa",
        "a",
        "aaaaa"
    };
    std::string s = "a{2,3}";

    Regex rv(s);
    ASSERT_EQ(rv.compile(15), 0);

    std::vector<std::string> ans;
    rv.find_all(checking, ans);

    std::vector<std::string> expected = {"aaa","aaa","aa"};
    ASSERT_EQ(ans.size(), expected.size());
    for (size_t k = 0; k < expected.size(); ++k)
        EXPECT_EQ(ans[k], expected[k]);
}

// concatenation + union + closure
TEST(check_find_all, Complex)
{
    std::vector<std::string> checking = {
        "ababc",
        "cc",
        "abcabc"
    };
    std::string s = "(a|b)+c";

    Regex rv(s);
    ASSERT_EQ(rv.compile(16), 0);

    std::vector<std::string> ans;
    rv.find_all(checking, ans);

    std::vector<std::string> expected = {"ababc","abc","abc"};
    ASSERT_EQ(ans.size(), expected.size());
    for (size_t k = 0; k < expected.size(); ++k)
        EXPECT_EQ(ans[k], expected[k]);
}

// concatenation + union of substrings
TEST(check_find_all, SubstringUnion)
{
    std::vector<std::string> checking = {
        "abcbc",
        "bab"
    };
    std::string s = "ab|bc";

    Regex rv(s);
    ASSERT_EQ(rv.compile(17), 0);

    std::vector<std::string> ans;
    rv.find_all(checking, ans);


    std::vector<std::string> expected = {"ab","bc","ab"};
    ASSERT_EQ(ans.size(), expected.size());
    for (size_t k = 0; k < expected.size(); ++k)
        EXPECT_EQ(ans[k], expected[k]);
}

TEST(check_find_all, Exact3)
{
    std::vector<std::string> checking = { 
        "aaaa",
        "aaa",
        "aa"
    };
    std::string s = "a{3}";

    Regex rv(s);
    ASSERT_EQ(rv.compile(18), 0);

    std::vector<std::string> ans;
    rv.find_all(checking, ans);

    std::vector<std::string> expected = {"aaa","aaa"};
    ASSERT_EQ(ans.size(), expected.size());
    for (size_t k = 0; k < expected.size(); ++k)
        EXPECT_EQ(ans[k], expected[k]);
}

TEST(check_invert, SimpleReverse) {
    Regex rv("ab");
    ASSERT_EQ(rv.compile(19), 0);

    std::vector<std::string> out1;
    rv.find_all({ "ab", "ba", "cab", "abc", "xaxb" }, out1);
     std::vector<std::string> exp1 = { "ab", "ab", "ab" };
    ASSERT_EQ(out1.size(), exp1.size());
    for (size_t i = 0; i < exp1.size(); ++i) {
        EXPECT_EQ(out1[i], exp1[i]);
    }


    Regex inv = rv.invert();
    std::vector <std::string> out2;
    inv.find_all({ "ab", "ba", "cab", "abc", "xaba" }, out2);

    std::vector<std::string> exp2 = { "ba", "ba" }; 
    ASSERT_EQ(out2.size(), exp2.size());
    for (size_t i = 0; i < exp2.size(); ++i) {
        EXPECT_EQ(out2[i], exp2[i]);
    }
}

//  (ab)+ to (ba)+
TEST(check_invert, ClosureReverse)
{
    Regex rv("(ab)+");
    ASSERT_EQ(rv.compile(20), 0);

    std::vector<std::string> orig;
    rv.find_all({ "xababx", "ab", "aba" }, orig);
    std::vector<std::string> exp_orig = { "abab", "ab", "ab" };
    ASSERT_EQ(orig.size(), exp_orig.size());
    for (size_t i = 0; i < exp_orig.size(); ++i) {
        EXPECT_EQ(orig[i], exp_orig[i]);
    }

    Regex inv = rv.invert();
    std::vector<std::string> rev;
    inv.find_all({ "xbabax", "ba", "bab" }, rev);
    std::vector<std::string> exp_rev = { "baba", "ba", "ba" };
    ASSERT_EQ(rev.size(), exp_rev.size());
    for (size_t i = 0; i < exp_rev.size(); i++)
    {
        EXPECT_EQ(rev[i], exp_rev[i]);
    }
}


static void CheckRecoverEquivalence(const std::string &orig_pattern,
                                    const std::vector<std::string>& yes,
                                    const std::vector<std::string>& no)
{

    Regex orig(orig_pattern);
    ASSERT_EQ(orig.compile(21), 0) << "Compile failed for: " << orig_pattern;

    std::string rec_pattern = orig.recover();

    Regex rec(rec_pattern);
    
    ASSERT_EQ(rec.compile(), 0) << "Compile failed for recovered: " << rec_pattern;

    for (auto& s : yes)
    {
        std::vector<std::string> out1, out2;
        orig.find_all({s}, out1);
        rec.find_all({s}, out2);
        EXPECT_FALSE(out1.empty()) << "Orig should match “" << s << "” for “"
                                  << orig_pattern << "”";
        EXPECT_FALSE(out2.empty()) << "Rec   should match “" << s << "” for “"
                                  << rec_pattern << "”";
    }
    for (auto& s : no)
    {
        std::vector<std::string> out1, out2;
        orig.find_all({s}, out1);
        rec.find_all({s}, out2);
        EXPECT_TRUE(out1.empty()) << "Orig should NOT match “" << s << "” for “"
                                 << orig_pattern << "”";
        EXPECT_TRUE(out2.empty()) << "Rec   should NOT match “" << s << "” for “"
                                 << rec_pattern << "”";
    }
}

TEST(check_recover, Literal) // Проверить сами автоматы на эквив.
{
    CheckRecoverEquivalence(
        "abc",
        /* yes */ {"abc", "xabc", "abcx"},
        /* no  */ {"ab", "xbc", "ac"}
    );
}

TEST(check_recover, PlusClosure)
{
    CheckRecoverEquivalence(
        "(ab)+",
        /* yes */ {"ab", "abab", "xababx", "ababab", "aba", "abb"},
        /* no  */ {"a", ""}
    );
}

TEST(check_recover, Union)
{
    CheckRecoverEquivalence(
        "a|bc",
        /* yes */ {"a", "bc", "xax bc", "bcabc", "ab"},
        /* no  */ {"b", "c", ""}
    );
}

TEST(check_recover, Range2to3)
{
    CheckRecoverEquivalence(
        "a{2,3}",
        /* yes */ {"aa", "aaa", "xaaaa", "aaaaa"}, 
        /* no  */ {"a", ""}
    );
}

TEST(check_recover, Complex)
{
    CheckRecoverEquivalence(
        "(a|b)+c{2}",
        /* yes */ {"aaccc", "bbcc", "c cbbcc", "abbbcc"}, 
        /* no  */ {"ac", "abc", "abbc", "abababc", ""}
    );
}
