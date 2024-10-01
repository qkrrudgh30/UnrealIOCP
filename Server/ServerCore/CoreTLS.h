#pragma once

extern thread_local uint32 LThreadID;
// 스레드 아이디를 직접 만들어서 관리하고자 함.
// 물론 표준을 통해 아이디를 추출해서 쓸 순 있으나, 반환값이 너무 중구난방.
