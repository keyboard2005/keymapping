function onWindowClose() {
    // 结束KeyBoardMouseHook.exe
    Neutralino.os.execCommand("taskkill /f /im KeyBoardMouseHook.exe");
    Neutralino.app.exit();
}


Neutralino.init();

Neutralino.events.on("windowClose", onWindowClose);

