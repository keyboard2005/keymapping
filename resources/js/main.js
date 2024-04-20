function onWindowClose() {
    // 结束KeyBoardMouseHook.exe
    Neutralino.os.execCommand("taskkill /f /im KeyBoardMouseHook.exe");
    Neutralino.app.exit();
}


Neutralino.init();

Neutralino.events.on("ready", () => {
    console.log("Neutralino is ready!");
    // 显示窗口
    Neutralino.window.show();
})

Neutralino.events.on("windowClose", onWindowClose);

