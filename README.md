# Driving-Test-Simulation
🎉🎉🎉🎉🎉🎉🎉🎉🎉🎉
## 关于本仓库，很高兴你能来到此处，在这里我会向你介绍一些关于本仓库的一些细节!
`祝你玩的愉快`
- 本仓库使用主要使用`OpenGL`实现了一个车辆模拟的三维程序。
- 物理引擎方面使用现成的物理库Physx（关于为什么使用这个物理库，我可以为你讲上三天三夜，这里有太多的理由了......）。我引入此库实现了车辆的运动，并且支持手动挡和自动挡的切换驾驶。
- 画面镜头能够跟随`WASD`键移动（上帝视角），而且可以手动切换为车辆视角。按住右键移动可移动画面。
- 手写了一个日志系统，能够附加输出到控制台和文件中，用于调试和保存日志信息。

## 示例图片
<img width="1858" height="1080" alt="image" src="https://github.com/user-attachments/assets/40a3ce83-c875-4eb6-840e-f42d03c99322" />
<img width="1858" height="1080" alt="image" src="https://github.com/user-attachments/assets/ce6aa416-9abb-4fd4-94b5-6cb12df22265" />
<img width="1027" height="157" alt="image" src="https://github.com/user-attachments/assets/4a54a348-3005-43fe-a719-7c5514480581" />
<img width="1858" height="1080" alt="image" src="https://github.com/user-attachments/assets/18fce0d7-7c95-4cbd-a9cf-34d40c1b6d5c" />


## 关于未来
- 目前开发平台在Windows10上，暂不考虑跨平台设计，现阶段使用的CMake来组织代码，如果以后有更好的组织方式可以试试修改玩玩。
- 在仓库中已经部分实现了ECS实体组件系统，在未来几年需要慢慢完善这个系统，希望以后能够实现一个更加稳定高效的系统。
- 现在由于个人原因无法继续完成，后续更新也许拖延到2026年。

## 关于计算机图形学&人生寄语
本人初学不久，对低层开发比较感兴趣，希望能够继续坚持。在以后可能会接触能多新方面，比如逆向工程、数据库等方向，计算机图形学对于我来说其实并不是非常感兴趣，我希望以后能够接触更多有意思的方向或者学科。
我在这里送给我一句话：`也许我并不适合搞计算机，更有可能的是，我更适合成为一个科学家。`
 
## 通过git查看文件行数（忽略此处）
```cpp
 git ls-files | grep -E '\.(cpp|h|hpp|cxx|cc)$' | grep -v "thirdParty/" | grep -v "application/stb_image.h" | grep -v "imgui" | xargs wc -l
```
