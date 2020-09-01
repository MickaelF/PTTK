@echo off
for %%I in ("%~dp0.") do for %%J in ("%%~dpI.") do set ParentFolderName=%%~dpnxJ

call lupdate %ParentFolderName% -ts French.ts English.ts