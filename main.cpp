/*********************************************************************
* Software License Agreement (BSD License)
*
*  Copyright (c) 2014, Texas A&M University
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above
*     copyright notice, this list of conditions and the following
*     disclaimer in the documentation and/or other materials provided
*     with the distribution.
*   * Neither the name of the Texas A&M University nor the names of its
*     contributors may be used to endorse or promote products derived
*     from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
*  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
*  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
*  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
*  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
*  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
*  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
*  POSSIBILITY OF SUCH DAMAGE.
*********************************************************************/

/* Author: Saurav Agarwal */

#include "FIRM2DSetup.h"
#include "MultiModalSetup.h"
#include "Tests.h"
#include <QApplication>
#include <QtGui/QDesktopWidget>
#include "include/Visualization/Window.h"
#include "include/Visualization/Visualizer.h"
#include <boost/thread.hpp>
#include <iostream>
#include <istream>


using namespace std;

void plan()
{
    FIRM2DSetup *mySetup(new FIRM2DSetup);

    std::string setupFilePath = "./SetupFiles/Setup6CorridorWorld.xml";

    mySetup->setPathToSetupFile(setupFilePath.c_str());

    mySetup->setup();

    Visualizer::updateRenderer(*dynamic_cast<const ompl::app::RigidBodyGeometry*>(mySetup), mySetup->getGeometricStateExtractor());

    Visualizer::updateSpaceInformation(mySetup->getSpaceInformation());

    Visualizer::setMode(Visualizer::VZRDrawingMode::PRMViewMode);

    int mode = 2;

    OMPL_INFORM("Choose what mode (0: Standard FIRM, 1 : Rollout , 2: Kidnapping-Multi-Modal)? : ");

    //cin>>mode;

    int keepTrying = 1;

    mySetup->loadGraphFromFile();

    while(keepTrying)
    {
        if(mySetup->solve())
        {
            mySetup->executeSolution(mode);

            OMPL_INFORM("Plan Executed.");

            keepTrying = 0;

        }
        else
        {
            OMPL_INFORM("Unable to find Solution in given time, would you like to continue attempt. (1: yes, 0 :no) ? :");

            std::cin>>keepTrying;
        }

    }

    delete mySetup;

}

/*
void testMultiModal()
{

    MultiModalSetup *mySetup(new MultiModalSetup);

    std::string setupFilePath = "./SetupFiles/SetupICRA2015Exp1.xml";

    mySetup->setPathToSetupFile(setupFilePath.c_str());

    mySetup->setup();

    Visualizer::updateRenderer(*dynamic_cast<const ompl::app::RigidBodyGeometry*>(mySetup), mySetup->getGeometricStateExtractor());

    Visualizer::updateSpaceInformation(mySetup->getSpaceInformation());

    if(mySetup->solve())
    {

        OMPL_INFORM("Plan Executed Successfully");

    }
    else
    {
        OMPL_INFORM("Unable to find Solution in given time.");
    }

    delete mySetup;

    exit(0);
}
*/

int main(int argc, char *argv[])
{
    srand(2345);

    arma_rng::set_seed(2345);

    QApplication app(argc, argv);

    MyWindow window;

    window.resize(window.sizeHint());

    window.showMaximized();

    window.resetCamera();

    boost::thread solveThread(plan);

    app.exec();

    solveThread.join();

    OMPL_INFORM("Task Complete");

    return 0;


}
