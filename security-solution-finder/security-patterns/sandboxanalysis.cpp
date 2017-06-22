// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// Implementation of the SandboxAnalysis.

#include "sandboxanalysis.h"

SandboxAnalysis::SandboxAnalysis(shared_ptr<Sources> sources)
    : _sources(sources), _threshold(0.9)
{
    _findKeywords = shared_ptr<FindKeywords>(new FindKeywords(sources));
    _findAPICalls = shared_ptr<FindAPICalls>(new FindAPICalls());

    prepareKeywords();
    prepareAPICalls();
}

void SandboxAnalysis::doPreASTAnalysis()
{

}

void SandboxAnalysis::doPostASTAnalysis()
{
    _findKeywords->analyzeFilenames();

    _findKeywords->buildFileRating();
    _findAPICalls->buildFileRating();

    for(string file : _sources->sources())
    {
        double rating = 1.0;
        rating = rating * _findKeywords->getFileRating(file);
        rating = rating * _findAPICalls->getFileRating(file);
        rating = cbrt(rating);

        if(rating < 0.99)
        {
            _fileRatings.insert(pair<double,string>(rating,file));
        }
    }
}

vector<unique_ptr<ASTConsumer> > SandboxAnalysis::getASTConsumers(ASTContext *context)
{
    vector<unique_ptr<ASTConsumer> > consumers;

    unique_ptr<ASTConsumer> fkc = unique_ptr<ASTConsumer>(
                new FindKeywordsConsumer(context, _findKeywords));
    consumers.push_back(std::move(fkc));

    return consumers;
}

vector<unique_ptr<RecursiveVisitor> > SandboxAnalysis::getRecursiveVisitors(
        ASTContext *context)
{
    vector<unique_ptr<RecursiveVisitor> > visitors;

    unique_ptr<RecursiveVisitor> fkv = unique_ptr<RecursiveVisitor>(
                new FindKeywordsVisitor(context, _findKeywords));
    visitors.push_back(std::move(fkv));

    unique_ptr<RecursiveVisitor> fav = unique_ptr<RecursiveVisitor>(
                new FindAPIVisitor(context, _findAPICalls, _sources));
    visitors.push_back(std::move(fav));

    return visitors;
}

void SandboxAnalysis::printResults(raw_ostream &out) const
{
    out << "===\nResults for the Sandbox Pattern:\n===\n";
    auto it = _fileRatings.begin();

    while(it != _fileRatings.end() && it->first < _threshold)
    {
        out << "Possible Involvement in the Sandbox Pattern: (Rating: "
            << double_to_string((1.0 - it->first)*100) << " )\n"
            << it->second << "\n";
        _findKeywords->printResultsForFile(it->second, out);
        _findAPICalls->printResultsForFile(it->second, out);

        it++;
    }
    out << "\n\n\n";
}

void SandboxAnalysis::prepareKeywords()
{
    _findKeywords->addKeyword("sandbox",1,false);
    _findKeywords->addKeyword("broker",10,false);
    _findKeywords->addKeyword("target",10,false);
    _findKeywords->addKeyword("untrusted",2,false);
    _findKeywords->addKeyword("restrict",5,true);
    _findKeywords->addKeyword("privilege",4,false);
    _findKeywords->addKeyword("jail",2,false);
}

void SandboxAnalysis::prepareAPICalls()
{
    // Windows
    //Token security machanism
    string windowstoken = "Windows Token Security";
    _findAPICalls->addAPIHeader(windowstoken,"windows.h");
    _findAPICalls->addAPIHeader(windowstoken,"winbase.h");
    _findAPICalls->addAPIFunction("AdjustTokenPrivileges", windowstoken, 10);
    _findAPICalls->addAPIFunction("LookupPrivilegeValue", windowstoken, 10);
    _findAPICalls->addAPIFunction("SetEntriesInAcl", windowstoken, 10);
    _findAPICalls->addAPIFunction("CreateRestrictedToken", windowstoken, 10);
    _findAPICalls->addAPIFunction("SetTokenInformation", windowstoken, 10);

    //Job security mechanism
    string windowsjob = "Windows Job Security";
    _findAPICalls->addAPIHeader(windowsjob,"windows.h");
    _findAPICalls->addAPIHeader(windowsjob,"winbase.h");
    _findAPICalls->addAPIFunction("CreateJobObject", windowsjob, 10);
    _findAPICalls->addAPIFunction("AssignProcessToJobObject", windowsjob, 10);
    _findAPICalls->addAPIFunction("SetInformationJobObject", windowsjob, 10);
    _findAPICalls->addAPIFunction("SetNamedSecurityInfo", windowsjob, 10);
    _findAPICalls->addAPIFunction("SetSecurityInfo", windowsjob, 10);

    //Linux
    string prctl = "Linux prctl";
    _findAPICalls->addAPIHeader(prctl,"sys/prctl.h");
    _findAPICalls->addAPIFunction("prctl", prctl, 30);

    string setuid = "Linux setuid";
    _findAPICalls->addAPIHeader(setuid,"sys/types.h");
    _findAPICalls->addAPIHeader(setuid,"unistd.h");
    _findAPICalls->addAPIFunction("setuid",setuid,15);
    _findAPICalls->addAPIFunction("getuid",setuid,15);
    _findAPICalls->addAPIFunction("seteuid",setuid,15);
    _findAPICalls->addAPIFunction("geteuid",setuid,15);
    _findAPICalls->addAPIFunction("setreuid",setuid,15);
    _findAPICalls->addAPIFunction("getreuid",setuid,15);
    _findAPICalls->addAPIFunction("setresuid",setuid,15);
    _findAPICalls->addAPIFunction("getresuid",setuid,15);
    _findAPICalls->addAPIFunction("setgid",setuid,15);
    _findAPICalls->addAPIFunction("getgid",setuid,15);
    _findAPICalls->addAPIFunction("setegid",setuid,15);
    _findAPICalls->addAPIFunction("getegid",setuid,15);
    _findAPICalls->addAPIFunction("setregid",setuid,15);
    _findAPICalls->addAPIFunction("getregid",setuid,15);
    _findAPICalls->addAPIFunction("setresgid",setuid,15);
    _findAPICalls->addAPIFunction("getresgid",setuid,15);

    string usrns = "Linux user namespace";
    _findAPICalls->addAPIHeader(usrns,"sched.h");
    _findAPICalls->addAPIFunction("clone",usrns,15);
    _findAPICalls->addAPIFunction("unshare",usrns,15);
    _findAPICalls->addAPIFunction("setns",usrns,15);

    //Mac OS X
    string mac = "Mac OS X Sandbox";
    _findAPICalls->addAPIHeader(mac,"sandbox.h");
    _findAPICalls->addAPIFunction("sandbox_init", mac, 1);
    _findAPICalls->addAPIFunction("sandbox_free_error", mac, 1);

}

string SandboxAnalysis::double_to_string(double d) const
{
    stringstream ss;
    ss << std::fixed << std::setprecision(4) << d;
    return ss.str();
}
