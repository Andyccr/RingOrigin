// dark.js

/* ========== 最后更新时间功能 ========== */
function initLastUpdated() {
  const lastUpdated = document.getElementById('lastUpdated');
  if (lastUpdated) {
    lastUpdated.textContent = '最后更新: ' + new Date(document.lastModified).toLocaleString();
  }
}

/* ========== 暗色模式功能 ========== */
function initDarkMode() {
  const isDark = window.matchMedia('(prefers-color-scheme: dark').matches;
  document.body.classList.toggle('dark', isDark);
}

/* ========== 折叠文章功能 ========== 

function initPostCollapse() {
  document.querySelectorAll('.post-collapse').forEach(button => {
    button.addEventListener('click', () => {
      button.nextElementSibling.classList.toggle('hidden');
    });
  });
}
*/

/* ========== 初始化所有功能 ========== */
function initAll() {
  initLastUpdated();
  initDarkMode();
  /*initPostCollapse();*/
}

// 页面加载完成后执行
document.addEventListener('DOMContentLoaded', initAll);