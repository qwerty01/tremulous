#include <qcommon/cmd.h>

extern "C" bool rust_Cmd_CommandExists(const char* name);
extern "C" void rust_Cmd_AddCommand(const char* name, xcommand_t callback, completionFunc_t completion_callback);
extern "C" void rust_Cmd_RemoveCommand(const char* name);
extern "C" void rust_Cmd_RemoveCommandSafe(const char* name);
extern "C" void rust_Cmd_SetCommandCompletionFunc(const char* command, completionFunc_t callback);
extern "C" void rust_Cmd_CommandCompletion(void(*callback)(const char* s));
extern "C" void rust_Cmd_CommandCompletionSafe(void(*callback)(char* s));
extern "C" void rust_Cmd_CompleteArgument(const char* name, char* args, int arg_num);
extern "C" bool rust_Cmd_ExecuteCommand(const char* command);

bool Cmd_CommadExists(const char* name) {
    return rust_Cmd_CommandExists(name);
}
void Cmd_AddCommand(const char* name, xcommand_t callback) {
    return rust_Cmd_AddCommand(name, callback, nullptr);
}
void Cmd_RemoveCommand(const char* name) {
    return rust_Cmd_RemoveCommand(name);
}
void Cmd_RemoveCommandSafe(const char* name) {
    return rust_Cmd_RemoveCommandSafe(name);
}
void Cmd_SetCommandCompletionFunc(const char* command, completionFunc_t complete) {
    if (!complete)
        return;
    return rust_Cmd_SetCommandCompletionFunc(command, complete);
}
void Cmd_CommandCompletion(void(*callback)(const char*)) {
    if (!callback)
        return;
    return rust_Cmd_CommandCompletion(callback);
}
void Cmd_CommandCompletionSafe(void(*callback)(char*)) {
    if (!callback)
        return;
    return rust_Cmd_CommandCompletionSafe(callback);
}
void Cmd_CompleteArgument(const char* command, char* args, int argNum) {
    return rust_Cmd_CompleteArgument(command, args, argNum);
}
bool Cmd_ExecuteCommand(const char* command) {
    return rust_Cmd_ExecuteCommand(command);
}