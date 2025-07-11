Взаимодействие с игрой через консоль

Те фигуры, которые доступны для хода, подсвечиваются синим

![image](https://github.com/Jamy-Konstantinopol/chess/assets/119047289/c4f36a54-2d21-4238-a56f-6db9d9e1ec24)

Те клетки куда может ходить фигура подсвечиваются красным

![image](https://github.com/Jamy-Konstantinopol/chess/assets/119047289/e26c4b1b-56c5-49fb-bd92-f4ef8569d7d3)

Обработка неправильного ввода

![image](https://github.com/Jamy-Konstantinopol/chess/assets/119047289/892a5743-ddeb-4e5a-b19e-e43dba0e701a)

Последний ход подсвечивается:
 - Откуда - коричневый
 - Куда - желтый

![image](https://github.com/Jamy-Konstantinopol/chess/assets/119047289/3adf5307-7ea6-40bb-b026-050bff7de4a0)

Съеденные черные фигуры отображаются на белой стороне, а съеденные белые - на черной

![image](https://github.com/Jamy-Konstantinopol/chess/assets/119047289/e762680e-bc5d-4cfa-9709-0b40f0e354db)

Обработка ситуаций, когда ход вызовет шах

В данном случае белый король не может ходить, так как ему угрожает вражеская королева

![image](https://github.com/Jamy-Konstantinopol/chess/assets/119047289/19a56fbf-a535-45b9-b5fe-5123b0c2b248)

Завершение игры при "шахе и мате" или "ничье"

![image](https://github.com/Jamy-Konstantinopol/chess/assets/119047289/f3f10540-acc3-4c6b-a5a7-5cb4bebc6fb7)


Чтобы проект заработал:
- создать папку build в корне проекта
- и воспользоваться командой `cmake ..` в дериктории build
