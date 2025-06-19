// scripts/dark-mode.js
function initDarkMode() {
  // 1. 自动检测系统暗色模式
  const applySystemTheme = () => {
    const isDark = window.matchMedia('(prefers-color-scheme: dark)').matches;
    document.body.classList.toggle('dark', isDark);
  };

  // 2. 监听系统主题变化
  window.matchMedia('(prefers-color-scheme: dark)')
    .addEventListener('change', applySystemTheme);

  // 3. 初始化
  applySystemTheme();
}

// 导出函数（如果其他脚本需要调用）
export { initDarkMode };

// 自动执行（如果不需要模块化）
// initDarkMode();