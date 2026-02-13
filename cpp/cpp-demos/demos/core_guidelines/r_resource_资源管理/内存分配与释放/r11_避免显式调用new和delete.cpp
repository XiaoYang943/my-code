//
// Created by Administrator on 2026/1/18.
//

/*
 * 通用规则：避免显示调用 new 和 delete
 */
int main() {
    /*
     * 这段代码 隐含了一个强约束：
     * 每一次 new，都必须且只能有一次 delete，且路径正确、时机正确
     *
     * 但是这个约束在实际工程中非常容易出错
     */
    // T* p = new T(...);
    // ...
    // delete p;

    /*
     * 问题一：所有权不明确：
    * bar 会不会 delete f？
    * 调用者还需不需要 delete？
    * 多个函数都能看到 f，谁负责释放？
    */
    // Foo* f = new Foo();
    // bar(f);

    /*
     * 问题二：路径复杂时(return、throw、break...)，容易泄漏
     *
     */
    // Foo* f = new Foo();
    //
    // if (!init()) {
    //     return;   // !!! 泄漏
    // }
    //
    // do_something();
    //
    // delete f;    // !!!
}
