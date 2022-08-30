#include "driver.h"
#include "hook.h"

ULONG Driver::get_client_base(ULONG process_id)
{
    Definitions::INFO_STRUCT info = { 0 };
    info.code = Definitions::CODE_CLIENT_REQUEST;
    info.process_id = process_id;

    Hook::run_hook(&info);

    return info.client_base;
}
