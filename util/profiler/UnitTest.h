/*****************************************************************//**
 * \file   UnitTest.h
 * \brief  单元测试
 * 
 * \author Capp-Adocia
 * \site https://github.com/capp-adocia/
 * \date   March 2025
 *********************************************************************/
#ifndef UNITTEST_H
#define UNITTEST_H
#include "benchmark.h"
#include "../../core/EventBus.h"
#include "../../core/world/world.h"
#include "../../core/ecs/system/RenderSystem.h"

namespace UnitTest {
    //struct TestEvent {
    //    int value;
    //};

    //void listenerLow(TestEvent e) {
    //    std::cout << "Low priority listener: " << e.value << std::endl;
    //}

    //void listenerNormal(TestEvent e) {
    //    std::cout << "Normal priority listener: " << e.value << std::endl;
    //}

    //void listenerHigh(TestEvent e) {
    //    std::cout << "High priority listener: " << e.value << std::endl;
    //}
    //void func() {
    //    // 如果是testevent那么就会调用listenerLow
    //    EB_INS.Subscribe<TestEvent>(listenerLow, EventPriority::NORMAL);
    //    EB_INS.Subscribe<TestEvent>(listenerNormal, EventPriority::LOW);
    //    EB_INS.Subscribe<TestEvent>(listenerHigh, EventPriority::HIGH);

    //    // 发布事件
    //    TestEvent event{ 42 };
    //    EB_INS.Publish(event);
    //}

    static World world;
    using component::TagComponent;
    using component::TransformComponent;

    void func()
    {
        Entity entity = world.CreateEntity("111");
        Entity entity2 = world.CreateEntity("222");

        world.LoadWorld();
        world.OnTick(1 / 60.0f);

        /* 结束后 */
        world.DestroyEntity(entity);
        world.DestroyEntity(entity2);
    }




} // namespace UnitTest

namespace UnitTest {
    void RunAllTests()
    {
        func();
    }
} // namespace UnitTest
#define UNIT_TEST UnitTest::RunAllTests();
#endif // !UNITTEST_H