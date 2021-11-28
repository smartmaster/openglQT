#include "testtab.h"
#include "ui_testtab.h"

#include <SMLAxisSystem.h>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/string_cast.hpp>

#include <string>
#include <cassert>

TestTab::TestTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestTab)
{
    ui->setupUi(this);
}

TestTab::~TestTab()
{
    delete ui;
}

void TestTab::on_pushButtonTestAxisSystem_clicked()
{
    using namespace ::SmartLib;

    static constexpr double eps = 1.0e-6;

    AxisSystem<double> axisSys;

    auto scalar = glm::tvec3<double>{1.0/100, 1.0/200, 1.0/400};
    axisSys.Scale(scalar);

    auto radians = glm::radians<double>(90.0);
    auto rotAxis = glm::tvec3<double>{0.0, 0.0, 1.0};
    axisSys.Rotate(radians, rotAxis);


    auto offset = glm::tvec3<double>{100, 200, 400};
    axisSys.Translate(offset);

    auto model = glm::tvec3<double>{100, 200, 400};

    auto world = axisSys.ModelToWorld(model);
    assert(glm::all(glm::epsilonEqual(world, glm::tvec3<double>(-2, 2, 2), eps)));

    auto model1 = axisSys.WorldToModel(world);
    assert(glm::all(glm::epsilonEqual(model1, model, eps)));

    auto matm2w = axisSys.ModelToWorldMat();
    auto world1 = AxisSystem<double>::Mat4xVec3(matm2w, model);
    assert(glm::all(glm::epsilonEqual(world1, world, eps)));

    auto matw2m = axisSys.WorldToModelMat();
    auto model2 = AxisSystem<double>::Mat4xVec3(matw2m, world);
    assert(glm::all(glm::epsilonEqual(model2, model, eps)));


    ///////////////////////////////////////////////////
    double left = 0.234234;
    double right = 2334.234234;
    double bottom = -0.293472;
    double top = 575.234234;
    double near = 293.234234;
    double far = 440.24234;
    double up0 = 8585.345;
    double up1 = 289.997;
    double up2 = -488.00;


    

    auto matGlm = glm::ortho(left, right, bottom, top, near, far);
    auto matAS = AxisSystem<double>::Ortho(left, right, bottom, top, near, far);
    for(int ii = 0; ii< 4; ++ ii)
    {
        auto cmp = glm::epsilonEqual(matGlm[ii], matAS[ii], eps);
        std::string str = glm::to_string(cmp);
        assert(glm::all(cmp));
    }


    ///////////////////////////////////////////////////
    auto matEyeGlm = glm::lookAt(glm::tvec3<double>{left, right, bottom},
                          glm::tvec3<double>{top, near, far},
                          glm::tvec3<double>{up0, up1, up2});


    auto matEyeAS = AxisSystem<double>::LookAt(
                        glm::tvec3<double>{left, right, bottom},
                          glm::tvec3<double>{top, near, far},
                          glm::tvec3<double>{up0, up1, up2});

    for(int ii = 0; ii< 4; ++ ii)
    {
        auto cmp1= glm::epsilonEqual(matEyeGlm[ii], matEyeAS[ii], eps);
        std::string str = glm::to_string(cmp1);
        assert(glm::all(cmp1));
    }


}


