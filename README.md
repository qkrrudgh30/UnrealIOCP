- MMORPG 개발에 필요한 소켓 프로그래밍 및 네트워크 지식을 얻기 위해,   
  "C++ MMORPG 게임 서버 만들기" 강의와 "UE5 & IOCP 서버 연동" 강의를
  인프런을 통해 수강 했습니다.   
  이 레포지토리는 두 수업에서 배운 내용을 적용하여 만든 채팅 & 우편 시스템 포트폴리오입니다.
   
   
- 채팅창 구현   
  지식을 제대로 이해했는지 확인해보기 위해 채팅창을 가장 먼저 구현해보았습니다.   
  채팅은 일반 채팅과 명령어 채팅으로 나눠보았습니다. 이는 메이플스토리에서 영감을 받아 구현했습니다.   
  GameMaster 플레이어의 경우 명령어 채팅을 사용할 수 있고, 이를 통해 공지를 띄울 수도 있습니다.   
  임의의 유저에게 우편을 보낼 수 있고, 해당 우편을 런타임 중에 삭제도 가능하게끔 구현해보았습니다.   
  이를 통해 QA 분들도 쉽게 사용하실 수 있게끔 했습니다.
   
  
- 우편함 구현   
  대부분의 MMORPG 기능들이 "우편함"을 기반으로한 컨텐츠가 많다고 생각하여 구현해보았습니다.   
  채팅창은 ScrollBox로 구현했으나, 비교해보기 위해 우편함은 ListView로 구현했습니다.   
  만약 실수로 수만개의 우편을 보냈다 하더라도, 받는 플레이어에게는 N개씩만 보여지게끔 구현했습니다.   
  마치 무한스크롤처럼, 한 번에 수만개의 우편이 보이는게 아니라 요청할때마다 N개씩 보여지게끔   
  구현해서 최적화를 꾀했습니다.   
    
    
- 관련 영상 링크   
  https://www.youtube.com/watch?v=RNHZg-KX6zU 
