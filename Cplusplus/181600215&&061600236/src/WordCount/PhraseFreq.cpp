#include "stdafx.h"
#include "WordFreq.h"
#include "PhraseFreq.h"

vector < pair<string, int>> vecPhrase;
map<string, int> mapPhrase;
extern string outputFile;
extern int phraseLen;
extern int topN;
extern bool weightOnWord;
extern bool inTitle;

bool cmp2(pair<string, int> a, pair<string, int> b)
{
	if (a.second != b.second)
		return a.second > b.second;
	return a.first < b.first;
}

//count the phrase frequency of a line
void PhraseFreq(string s)
{
	string word = "";
	string phrase = "";
	int cnt = 0, secWord;
	int len = s.length();
	for (int i = 0; i < len; i++) {
		if (s[i] >= 'A' && s[i] <= 'Z') {
			s[i] += 32; //change into lower case
		}
		if ((s[i] >= '0'&&s[i] <= '9') || (s[i] >= 'a'&&s[i] <= 'z')) {
			word += s[i];
			if (i == len - 1 && IsWord(word)) {
				cnt++;
				if (cnt == 1) {
					phrase += word;
				}
				else {
					phrase += " ";
					phrase += word;
				}
				if (cnt == 2) {
					secWord = i - word.size()-1;
				}
				if (cnt == phraseLen) {
					if (weightOnWord && inTitle) {
						mapPhrase[phrase] += 10;
					}
					else
						mapPhrase[phrase]++;
					i = secWord;
				}
			}
		}
		else if(word!=""){
			if (IsWord(word)) {
				cnt++;
				if (cnt == 1) {
					phrase += word;
				}
				else {
					phrase += " ";
					phrase += word;
				}
				if (cnt == 2) {
					secWord = i - word.size()-1;
				}
				if (cnt == phraseLen) {
					if (weightOnWord && inTitle) {
						mapPhrase[phrase]+=10;
					}
					else
						mapPhrase[phrase]++;
					i = secWord;
					cnt = 0;
					phrase = "";
				}
			}
			else {
				cnt = 0;
				phrase = "";
			}
			word = "";
		}

	}
}

//output the first n phrases
void TopNPhrases()
{
	for (auto x : mapPhrase) {
		vecPhrase.push_back(x);
	}
	sort(vecPhrase.begin(), vecPhrase.end(), cmp2);
	ofstream fout(outputFile, ios::app);
	for (auto x : vecPhrase) {
		if (!topN) break;
		fout << "<" << x.first << ">: " << x.second << endl;
		topN--;
	}
	fout.close();
}