## gui 程序使用说明
### 运行配置
- 将 `core.dll` 与 `gui.exe` 放置同一目录下
- 启动 `gui.exe` 即可运行

### 界面说明
- 输入部分
  - 点击“导入文件”按钮可导入本地文件到输入栏中
  - 也可以在输入栏中直接编辑文本
- 输出部分
  - 运行后可在输出栏中看到结果，不可手动编辑结果
  - 点击“导出结果”可将结果导出到文件
- 选项部分
  - 有三种模式可以选择
    - 求所有单词链数目
    - 求单词数量最多的单词链
    - 求字母数量最多的单词链
  - 当选择的不是“求所有单词链数目”时，可以额外增加 4 种参数
    - 指定首字母
    - 指定尾字母
    - 指定非首字母
    - 允许隐含单词环
  - 点击“运行”按钮后开始计算
    - 如有错误会弹出提示框
    - 正确运行后会显示答案和运行时间