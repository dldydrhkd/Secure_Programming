# Miller-Rabin Algorithm

>### 개요

  * 목표 : 소수와 합성수를 판별하는 miller-rabin test 함수 구현
  
  * 요구사항
   
    - 추가적인 함수 정의 허용함(단, exp 함수는 직접 구현하지 말고 exponentiation.py에서 import 할 것)
   
    - 추가적인 외부 라이브러리 import 허용하지 않음
    
    - 개발환경 : mac환경에서 pycharm으로 python3를 interpreter로 설정하여 실행함
 
>### 필요한 자료구조 및 기능

  * 필요한 자료구조
   
    - miller_rabin 함수 구현
   
    - test 함수 구현
   
    - exp 함수 구현
 
>### 기능별 알고리즘 명세

  * 기능 구현
   
    - miller_rabin 함수 구현
   
      + 숫자가 들어왔을 때 짝수인지 홀수인지 판별하며 1 이하인 수가 들어오면 합성수와 소수를 반별 할 수 없으므로 Undefined 출력하게 함. 1 ~ n-1 사이의 숫자 a를 랜덤으로 뽑아 test함수에 넣어본다.
      
    - test 함수 구현
    
      + t >= 1 이고 u가 홀수인 n-1=(2^t)*u를 만족하는 u와 t를 구한다.
      
      + x 변수를 생성하고 a의 u승 mod n을 계산한다.
     
      + x가 1이거나 n-1이면 Prime임을 알린다 (페라마 소 정리)
      
      + 1부터 t-1까지 반복       (NSR)
      
        * x 후 = (x 전)^2 mod n 함
        
        * x 가 n-1 이면 break을 하여 Composite임을 알림
      
>### 인터페이스 및 사용법

  * 사용법
  
    - miller_rabin.py와 exponentiation.py를 같은 폴도에 넣고 miller_rabin.py를 실행시킨다.

>### 평가 및 개선 방향
  
  * 본 결과의 단점
  
    - miller_rabin 알고리즘의 경우 속도가 매우 빠르지만 틀릴 가능성이 없지 않은 확률적인 알고리즘이기 때문에 정확하고 빠른 알고리즘을 개발하고 공부하면 좋을거 같습니다.
          

 
