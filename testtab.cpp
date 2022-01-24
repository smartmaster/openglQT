#include "testtab.h"
#include "ui_testtab.h"

#include <SMLAxisSystem.h>
#include <glm/gtc/epsilon.hpp>
#include <glm/gtx/string_cast.hpp>

#include <random>

#include <string>
#include <vector>
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
    auto world1 = AxisSystem<double>::M4xP3(matm2w, model);
    assert(glm::all(glm::epsilonEqual(world1, world, eps)));

    auto matw2m = axisSys.WorldToModelMat();
    auto model2 = AxisSystem<double>::M4xP3(matw2m, world);
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


static void FillRandom(float* buiffer, int count)
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(-999999, 999999);
    int ii = 0;
    while(ii < count)
    {
        int rn = dist(mt);
        if(rn)
        {
            buiffer[ii] = float(rn)/(100000.0f);
            ++ii;
        }
    }
}

void TestTab::on_pushButtonTestAll_clicked()
{
    using T = float;
    float eps = glm::epsilon<float>()*1000;
    std::vector<float> buffer(4096);
    FillRandom(buffer.data(), (int)buffer.size());

    int index = 0;
    ////////////////////////////////////
    {

        auto m1 = SmartLib::AxisSystem<float>::Ortho(
                    buffer[index+0],
                buffer[index+1],
                buffer[index+2],
                buffer[index+3],
                buffer[index+4],
                buffer[index+5]);

        auto m2 = glm::ortho(
                    buffer[index+0],
                buffer[index+1],
                buffer[index+2],
                buffer[index+3],
                buffer[index+4],
                buffer[index+5]);

        index += 6;

        for(int ii = 0; ii < 3; ++ ii)
        {
            auto iseq = glm::epsilonEqual(m1[ii], m2[ii], eps);
            auto ok = glm::all(iseq);
            assert(ok);
        }


    }

    ///////////////////////////////////////
    {
        glm::tvec3<T> eye{buffer[index+0],
                    buffer[index+1],
                    buffer[index+2]};
        index += 3;

        glm::tvec3<T> center{buffer[index+0],
                    buffer[index+1],
                    buffer[index+2]};
        index += 3;

        glm::tvec3<T> up{buffer[index+0],
                    buffer[index+1],
                    buffer[index+2]};
        index += 3;

        auto m1 = SmartLib::AxisSystem<float>::LookAt(eye, center, up);
        auto m2 = glm::lookAt(eye, center, up);

        for(int ii = 0; ii < 3; ++ ii)
        {
            auto iseq = glm::epsilonEqual(m1[ii], m2[ii], eps);
            auto ok = glm::all(iseq);
            assert(ok);
        }
    }


    /////////////////////////////////
    {
        glm::tmat4x4<float> m4(
                    buffer[index+0],
                buffer[index+1],
                buffer[index+2],
                buffer[index+3],
                buffer[index+4],
                buffer[index+5],
                buffer[index+6],
                buffer[index+7],
                buffer[index+8],
                buffer[index+9],
                buffer[index+10],
                buffer[index+11],
                buffer[index+12],
                buffer[index+13],
                buffer[index+14],
                buffer[index+15]
                );
        index += 16;


        glm::vec4 v4{buffer[index+0],
                    buffer[index+1],
                    buffer[index+2],
                    buffer[index+3]};
        index += 4;

        {

            auto vec =  SmartLib::AxisSystem<T>::M4xV4(m4, v4);
            std::string str = glm::to_string(vec);
        }

        glm::vec3 v3{buffer[index+0],
                    buffer[index+1],
                    buffer[index+2]};
        index += 3;

        {
            glm::tvec3<T> vec = SmartLib::AxisSystem<T>::M4xV3(m4, v3);
            std::string str = glm::to_string(vec);
        }

        {
            glm::tvec3<T> vec = SmartLib::AxisSystem<T>::M4xP3(m4, v3);
            std::string str = glm::to_string(vec);
        }

        {
            glm::tvec4<T> vec = SmartLib::AxisSystem<T>::V3ToV4(v3);
            std::string str = glm::to_string(vec);
        }

        {
            glm::tvec4<T> vec = SmartLib::AxisSystem<T>::V3ToP4(v3);
            std::string str = glm::to_string(vec);
        }

        {
            glm::tvec3<T> vec = SmartLib::AxisSystem<T>::V4ToV3(v4);
            std::string str = glm::to_string(vec);
        }

        {
            glm::tvec3<T> originPos{buffer[index+0],
                        buffer[index+1],
                        buffer[index+2]};
            index += 3;
            glm::tvec3<T> horizontalV{buffer[index+0],
                        buffer[index+1],
                        buffer[index+2]};
            index += 3;
            glm::tvec3<T> verticalV{buffer[index+0],
                        buffer[index+1],
                        buffer[index+2]};
            index += 3;
            glm::tvec3<T> zV = glm::cross(horizontalV, verticalV);
            verticalV = glm::cross(zV, horizontalV);
            SmartLib::AxisSystem<T> as;

            as.MakeFromOHVZ(originPos, horizontalV, verticalV, zV);

            ///////////////////////
            glm::vec3 model{ buffer[index + 0],
                        buffer[index + 1],
                        buffer[index + 2] };
            index += 3;
            auto world = as.ModelToWorld(model);
            auto model1 = as.WorldToModel(world);
            auto world1 = as.ModelToWorld(model1);
            assert(glm::all(glm::epsilonEqual(model1, model, eps)));
            assert(glm::all(glm::epsilonEqual(world1, world, eps)));

            auto w2mmat = as.WorldToModelMat();
            auto m2wmat = as.ModelToWorldMat();
            auto world2 = SmartLib::AxisSystem<T>::M4xP3(m2wmat, model1);
            auto model2 = SmartLib::AxisSystem<T>::M4xP3(w2mmat, world2);

            assert(glm::all(glm::epsilonEqual(model1, model2, eps)));
            assert(glm::all(glm::epsilonEqual(world1, world2, eps)));
        }

        {
            glm::tvec3<T> originPos{buffer[index+0],
                        buffer[index+1],
                        buffer[index+2]};
            index += 3;
            glm::tvec3<T> horizontalPos{buffer[index+0],
                        buffer[index+1],
                        buffer[index+2]};
            index += 3;
            glm::tvec3<T> verticalPos{buffer[index+0],
                        buffer[index+1],
                        buffer[index+2]};
            index += 3;
            SmartLib::AxisSystem<T> as;
            as.MakeFromOHVPos(originPos, horizontalPos, verticalPos);


            ///////////////////////
            glm::vec3 model{ buffer[index + 0],
                        buffer[index + 1],
                        buffer[index + 2] };
            index += 3;
            auto world = as.ModelToWorld(model);
            auto model1 = as.WorldToModel(world);
            auto world1 = as.ModelToWorld(model1);
            assert(glm::all(glm::epsilonEqual(model1, model, eps)));
            assert(glm::all(glm::epsilonEqual(world1, world, eps)));

            auto w2mmat = as.WorldToModelMat();
            auto m2wmat = as.ModelToWorldMat();
            auto world2 = SmartLib::AxisSystem<T>::M4xP3(m2wmat, model1);
            auto model2 = SmartLib::AxisSystem<T>::M4xP3(w2mmat, world2);

            assert(glm::all(glm::epsilonEqual(model1, model2, eps)));
            assert(glm::all(glm::epsilonEqual(world1, world2, eps)));

        }

        {
            glm::tvec3<T> originPos{buffer[index+0],
                        buffer[index+1],
                        buffer[index+2]};
            index += 3;
            glm::tvec3<T> horizontalV{buffer[index+0],
                        buffer[index+1],
                        buffer[index+2]};
            index += 3;
            glm::tvec3<T> verticalV{buffer[index+0],
                        buffer[index+1],
                        buffer[index+2]};
            index += 3;
            SmartLib::AxisSystem<T> as;
            as.MakeFromOHV(originPos, horizontalV, verticalV);

            ///////////////////////
            glm::vec3 model{ buffer[index + 0],
                        buffer[index + 1],
                        buffer[index + 2] };
            index += 3;
            auto world = as.ModelToWorld(model);
            auto model1 = as.WorldToModel(world);
            auto world1 = as.ModelToWorld(model1);
            assert(glm::all(glm::epsilonEqual(model1, model, eps)));
            assert(glm::all(glm::epsilonEqual(world1, world, eps)));

            auto w2mmat = as.WorldToModelMat();
            auto m2wmat = as.ModelToWorldMat();
            auto world2 = SmartLib::AxisSystem<T>::M4xP3(m2wmat, model1);
            auto model2 = SmartLib::AxisSystem<T>::M4xP3(w2mmat, world2);

            assert(glm::all(glm::epsilonEqual(model1, model2, eps)));
            assert(glm::all(glm::epsilonEqual(world1, world2, eps)));

        }


        {
            glm::tvec3<T> originPos{buffer[index+0],
                        buffer[index+1],
                        buffer[index+2]};
            index += 3;
            glm::tvec3<T> horizontalV{buffer[index+0],
                        buffer[index+1],
                        buffer[index+2]};
            index += 3;
            glm::tvec3<T> verticalV{buffer[index+0],
                        buffer[index+1],
                        buffer[index+2]};
            index += 3;
            SmartLib::AxisSystem<T> as;
            as.MakeFromOHV(originPos, horizontalV, verticalV);

            ///////////////////////
            glm::vec3 model{ buffer[index + 0],
                        buffer[index + 1],
                        buffer[index + 2] };
            index += 3;
            auto world = as.ModelToWorld(model);
            auto model1 = as.WorldToModel(world);
            auto world1 = as.ModelToWorld(model1);
            assert(glm::all(glm::epsilonEqual(model1, model, eps)));
            assert(glm::all(glm::epsilonEqual(world1, world, eps)));

            auto w2mmat = as.WorldToModelMat();
            auto m2wmat = as.ModelToWorldMat();
            auto world2 = SmartLib::AxisSystem<T>::M4xP3(m2wmat, model1);
            auto model2 = SmartLib::AxisSystem<T>::M4xP3(w2mmat, world2);

            assert(glm::all(glm::epsilonEqual(model1, model2, eps)));
            assert(glm::all(glm::epsilonEqual(world1, world2, eps)));

        }

        {
            using T = double;
            glm::tvec3<T> originPos{buffer[index+0],
                        buffer[index+1],
                        buffer[index+2]};
            index += 3;
            glm::tvec3<T> horizontalV{buffer[index+0],
                        buffer[index+1],
                        buffer[index+2]};
            index += 3;
            glm::tvec3<T> verticalV{buffer[index+0],
                        buffer[index+1],
                        buffer[index+2]};
            index += 3;
            SmartLib::AxisSystem<T> asTemp;
            asTemp.MakeFromOHV(originPos, horizontalV, verticalV);

            /////////////////////////////////////
            SmartLib::AxisSystem<T> as;
            as.SetAxis(asTemp.GetAxis());
            as.SetScale(asTemp.GetScale());
            as.SetOrigin(asTemp.GetOrigin());

            glm::tvec3<T> scalar{buffer[index + 0], buffer[index + 1], buffer[index + 2]};
            index += 3;
            as.SetScale(scalar);

            glm::tvec3<T> origin{buffer[index + 0], buffer[index + 1], buffer[index + 2]};
            index += 3;
            as.SetOrigin(origin);


            const int loopCount = 5;
            for(int ii = 0; ii < loopCount; ++ ii)
            {

                glm::tvec3<T> scalar1{buffer[index + 0], buffer[index + 1], buffer[index + 2]};
                index += 3;
                as.Scale(scalar1);

                T radians = buffer[index + 0];
                index += 1;
                glm::tvec3<T> rotAxis{buffer[index + 0], buffer[index + 1], buffer[index + 2]};
                index += 3;
                as.RotateAbsolutely(radians, rotAxis);


                T radians1 = buffer[index + 0];
                index += 1;
                glm::tvec3<T> rotAxis1{buffer[index + 0], buffer[index + 1], buffer[index + 2]};
                index += 3;
                as.Rotate(radians1, rotAxis1);


                glm::tvec3<T> offset{buffer[index + 0], buffer[index + 1], buffer[index + 2]};
                index += 3;
                as.TranslateAbsolutely(offset);

                glm::tvec3<T> offset1{buffer[index + 0], buffer[index + 1], buffer[index + 2]};
                index += 3;
                as.Translate(offset);
            }



            //////////////////////////
            glm::tvec3<T> model{buffer[index+0],
                        buffer[index+1],
                        buffer[index+2]};
            index += 3;

            auto world = as.ModelToWorld(model);
            auto model1 = as.WorldToModel(world);
            auto world1 = as.ModelToWorld(model1);
            assert(glm::all(glm::epsilonEqual(model1, model, (T)eps)));
            assert(glm::all(glm::epsilonEqual(world1, world, (T)eps)));

            auto w2mmat = as.WorldToModelMat();
            auto m2wmat = as.ModelToWorldMat();
            auto world2 = SmartLib::AxisSystem<T>::M4xP3(m2wmat, model1);
            auto model2 = SmartLib::AxisSystem<T>::M4xP3(w2mmat, world2);

            assert(glm::all(glm::epsilonEqual(model1, model2, (T)eps)));
            assert(glm::all(glm::epsilonEqual(world1, world2, (T)eps)));


        }

    }

}


void TestTab::on_pushButton_test_rotmat_clicked()
{
    using T = float;
    float eps = glm::epsilon<T>()*1000;
    std::vector<float> buffer(128);
    FillRandom(buffer.data(), (int)buffer.size());

    T angle = buffer[0];
    glm::tvec3<T> raxis{buffer[1], buffer[2], buffer[3]};
    static  glm::tmat4x4<T> matE4{1};
    auto glmmat4 = glm::rotate(matE4, angle, raxis);
    glm::tmat3x3<T> glmmat3
    {
        glm::tvec3<T>{glmmat4[0]},
        glm::tvec3<T>{glmmat4[1]},
        glm::tvec3<T>{glmmat4[2]},
    };
    auto mymat = SmartLib::AxisSystem<T>::RotateMat(angle, raxis);

    auto glmmat3str = glm::to_string(glmmat3);
    auto mymatstr = glm::to_string(mymat);

    for(int ii = 0; ii < 3; ++ ii)
    {
        qDebug() << glm::to_string(glmmat3[ii]).c_str();
        qDebug() << glm::to_string(mymat[ii]).c_str();
        auto bvec = glm::epsilonEqual(glmmat3[ii], mymat[ii], eps);
        auto bvecstr = glm::to_string(bvec);
        qDebug() << bvecstr.c_str();
        assert(glm::all(bvec));
    }
}

