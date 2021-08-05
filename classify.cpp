#include <map>
#include <string>
#include <iostream>
#include <set>
#include <utility>
#include <math.h>
#include "csvstream.h"

using namespace std;

class Label {
public:
    Label();

private:

    int numWLabel;
};

class Post {
public:

    //returns set of unique words
    std::set<string> getWords() {
        return uniqueWords;
    }

    //adds word to set if word is not already in there
    void addWords(string content) {

        ogContent = content;
        stringstream ss(content);
        string in;
        while (ss >> in) {
            uniqueWords.insert(in);
        }

    }

    void setCorrectLabel(string label) {
        correctLabel = label;
    }

    string getCorrectLabel() {
        return correctLabel;
    }

    string getOgContent() {
        return ogContent;
    }

    void setProb(double probability) {
        prob = probability;
    }

    double getProbability() {
        return prob;
    }

private:

    std::set<string> uniqueWords;
    string correctLabel;
    string ogContent;
    double prob;

};

class Classifier {
public:

    Classifier(string train_file, string test_file, bool buggy) : totalPosts(0),
    tester(test_file), trainer(train_file), bug(buggy) { };

    bool train() {

        map<string, string> junk;
        string sendToPost;
        vector<Post> localVector;
        
        try {
            csvstream csv_train(trainer);
            while (csv_train >> junk ) {
                auto content = junk.find("content");
                auto tag = junk.find("tag");
                Post newPost;
                newPost.addWords(content->second);
                newPost.setCorrectLabel(tag->second);
                localVector.push_back(newPost);
                junk.clear();
            }

        } catch (csvstream_exception e) {
            cout << e.what() << endl;
            return 0;
        }
    
        //iterates through local vector to analyze data
        for (int i = 0; i < (int)localVector.size(); ++i) {
            //current post
            Post thisPost = localVector[i];
            //current post's unique words
            std::set<string> thisPostsWords = thisPost.getWords();
            //current post's label
            string thisPostsLabel = localVector[i].getCorrectLabel();
            
            //iterates through current post's unique words
            for (string w : thisPostsWords) {
                //adds words for this post to big list of all unique words
                ++postWord[w];
                //add words to unique words for that post
                ++(labelPosts[thisPostsLabel])[w];
            }
            //adds label to label map or increments label count
            ++label[thisPostsLabel];
        }
        
        totalPosts = (int)localVector.size();
        uniqueWords = (int)postWord.size();
        
        return 1;
        
    }

    double computeLogPrior(string C) {

        int numPostsC;
        double logPrior;

        auto r = label.find(C);
        numPostsC = r->second;

        logPrior = log((double)numPostsC / totalPosts);
        return logPrior;
    }

    double computeLogLike(string w, string C) {
        double numPostsC;
        double numWordC;
        double logLike;
        auto i = labelPosts.find(C);
        auto q = (i->second).find(w);
        if (q == (i->second).end()) {
            numWordC = 0;
        }
        else {
           numWordC = q->second;
        }

        auto r = label.find(C);
        numPostsC = r->second;

        auto l = postWord.find(w);
        if (numWordC == 0 && l == postWord.end()) {
            logLike = log(1.0 / totalPosts);
        }

        else if (numWordC == 0 && l != postWord.end()) {
            logLike = log((double)l->second / (double)totalPosts);
        }

        else {
            logLike = log((double)numWordC / numPostsC);
        }

        return logLike;
    }

    //needs to have builder for Post object set
    bool buildPosts() {
        
        map<string, string> junk;
        string sendToPost;

        try {
            csvstream csv_test(tester);
            while (csv_test >> junk ) {
                auto content = junk.find("content");
                auto tag = junk.find("tag");
                Post newPost;
                newPost.addWords(content->second);
                newPost.setCorrectLabel(tag->second);
                testPosts.push_back(newPost);
                junk.clear();
            }
        } catch (csvstream_exception e) {
            cout << e.what() << endl;
            return 1;
        }
        
        return 0;
    }

    //Function that takes in a post object and label to compute likelihood of
    //that label given that post
    //"computeLikelihood"

    double computeProbability(Post myPost, string label) {
        set<string> words = myPost.getWords();
        double sum = computeLogPrior(label);

        for(auto i : words) {
            sum += computeLogLike(i, label);
        }

        return sum;
    }


    //calls function computeLikelihood, then takes the max of those results to
    //return prediction,
    //let's call it "computePrediction"
    string computePrediction(Post &myPost) {
        double max = -10000000;
        string prediction = "error";

        for (auto i : label) {
            double llHood = computeProbability(myPost, i.first);

            if (llHood > max) {
                max = llHood;
                prediction = i.first;
            }

        }
        myPost.setProb(max);
        return prediction;
    }

    //calls "computePrediction" on each post object in vector and prints out
    //prediction
    bool testData() {
        int numCorrect = 0;
        
        if (buildPosts()) {
            return 0;
        }
        
        if (bug == true) {
            debug();
        }
        else {
            cout << "trained on " << totalPosts << " examples" << endl;
        }
        cout << endl;
        if (bug == true) {
            cout << "classes:" << endl;
            for (auto i : label){
                 cout << "  " << i.first << ", " << i.second
                << " examples, log-prior = " << computeLogPrior(i.first)
                << endl;
            }
            cout << "classifier parameters:" << endl;
            for (auto i : labelPosts) {
                for (auto w : i.second) {
                cout << "  " << i.first << ":" << w.first << ", count = " <<
                    w.second << ", log-likelihood = " <<
                    computeLogLike(w.first, i.first) << endl;
                }
            }
            cout << endl;
        }
        cout << "test data:" << endl;
        for (int i = 0; i < (int)testPosts.size(); ++i) {
            string correctLabel = testPosts[i].getCorrectLabel();
            string myLabel = computePrediction(testPosts[i]);
            double logProb = testPosts[i].getProbability();
            cout << "  correct = " << correctLabel << ", predicted = "
            << myLabel << ", log-probability score = "
                    << logProb << endl;

            cout << "  content = " << testPosts[i].getOgContent() << endl
            << endl;

            if (correctLabel == myLabel) numCorrect++;
        }
        cout << "performance: " << numCorrect << " / " << testPosts.size() <<
        " posts predicted correctly" << endl;
        
        return 1;
    }


    void debug() {
        csvstream csv_train(trainer);
        cout << "training data:" << endl;
        map<string, string> row;
        while (csv_train >> row) {
            string key = row["tag"];
            string content = row["content"];
            cout << "  label = " << key << ", content = " << content << endl;
        }
        cout << "trained on " << totalPosts << " examples" << endl;
        cout << "vocabulary size = " << uniqueWords << endl;

    }


private:
    int totalPosts;
    int uniqueWords;
    map<string, int> postWord;
    map<string, int> label;
    map<string, vector<string> > wordsInLabel;
    //first is lable, second is a map of unique words in that label and num
    //occurrences
    map<string, map<string, int > > labelPosts;
    string tester;
    string trainer;
    bool bug;
    vector<Post> testPosts;
};

int printError();
int main (int argc, char ** argv) {
    cout.precision(3);

    string trainFile = argv[1];
    string testFile = argv[2];
    bool bug = false;
    string debug;
    if (argc == 4) debug = argv[3];

    if (argc < 3 || argc > 4) {
        printError();
    }

    if (argc == 4 && debug != "--debug") {
        printError();
    }

    if (argc == 4) {
        bug = true;
    }
    
    Classifier classify(trainFile, testFile, bug);
    
    if (!classify.train()) {
        return 1;
    }
    
    if (!classify.testData()) {
        return 1;
    }

}

int printError() {
    cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
    return 1;
}
