// Copyright (c) 2017 Markus Schablack. All rights reserved.
// This file is distributed under the University of Illinios Open Source License.
// Details can be found in the LICENSE file.
//====
// This file implements the MainAnalysis class.

#include "mainanalysis.h"

MainAnalysis::MainAnalysis(ClangTool tool, shared_ptr<Sources> sources)
    : _tool(tool), _sources(sources)
{

}

void MainAnalysis::addPatternAnalysis(shared_ptr<PatternAnalysis> pa)
{
    _analyses.push_back(pa);
}

int MainAnalysis::startAnalysis()
{
    for(shared_ptr<PatternAnalysis> pa : _analyses)
    {
        pa->doPreASTAnalysis();
    }

    int result = _tool.run(getFrontendActionFactory().get());

    for(shared_ptr<PatternAnalysis> pa : _analyses)
    {
        pa->doPostASTAnalysis();
    }

    return result;
}

void MainAnalysis::printResults(raw_ostream &out) const
{
    for(shared_ptr<PatternAnalysis> pa : _analyses)
    {
        pa->printResults(out);
    }
}

std::unique_ptr<FrontendActionFactory> MainAnalysis::getFrontendActionFactory()
{
    ConsumerFactory cfactory (_analyses);
    VisitorFactory vfactory (_analyses);
    class SimpleFrontendActionFactory : public FrontendActionFactory
    {
    public:
        SimpleFrontendActionFactory(ConsumerFactory cfactory,
                                    VisitorFactory vfactory,
                                    shared_ptr<Sources> sources)
            : _cfactory(cfactory), _vfactory(vfactory), _sources(sources) {}
        clang::FrontendAction *create() override {
            return new MainAnalysisAction(_cfactory, _vfactory, _sources); }
    private:
        ConsumerFactory _cfactory;
        VisitorFactory _vfactory;
        shared_ptr<Sources> _sources;
    };

    return std::unique_ptr<FrontendActionFactory>(
                new SimpleFrontendActionFactory(cfactory,vfactory,_sources));

}
