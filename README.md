# WordList

- 维持 `main` 主分支，这个分支应该是没有 bug 的，稳定的。
- 开发的时候从 `main` 上 `checkout` 出新的开发分支。
- 合并分支的时候应当先在 `main` 上 `pull`，更新 `main` 分支（`main` 可能被其他人更新）。然后切换到开发分支，`merge main` 。当功能正常以后，切换到 `main` 分支，然后 `merge dev` 。最后 `push main` 完成一次开发。
- 尽量多 `push` 更新。