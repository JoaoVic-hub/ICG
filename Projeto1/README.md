# Aula prática 02 - Visualização 3D no OpenGL

| Arquivo | Exercício |
|---|---|
| `cube_ex1_1.c` | 1.1 - `gluLookAt()` x `glTranslatef(0, 0, -5)`, outros parâmetros do `gluLookAt()` e câmera vendo o cubo de cima |
| `cube_ex1_2.c` | 1.2 - `glFrustum()`, `gluPerspective(60, 1, 1.5, 20)` e `glOrtho()` |
| `robot.c` | 2.1 - braço robótico (base, ombro, cotovelo, punho e garra) |

Os três partem do `cube.c` e do `robot.c` do pacote `redbook.zip` da disciplina.
As teclas de cada programa estão no comentário do início do arquivo.

Teclas do `robot.c` (minúscula/maiúscula giram em sentidos opostos):
`b` base, `s` ombro, `e` cotovelo, `w` flexão do punho, `t` giro do punho,
`f` abre/fecha a garra, `ESC` sai.

## Compilar

**Windows (MSYS2):** instale o freeglut no terminal MSYS2 MINGW64 com
`pacman -S mingw-w64-x86_64-freeglut` e rode `compilar.bat` nesta pasta.
Para executar fora do terminal do MSYS2, `C:\msys64\mingw64\bin` precisa
estar no PATH (por causa da `libfreeglut.dll`).

**Linux** (comando do roteiro):

```
g++ robot.c -o robot -lGL -lGLU -lglut && ./robot
```

## Respostas

**1.1 - Por que `gluLookAt(0,0,5, 0,0,0, 0,1,0)` e `glTranslatef(0,0,-5)` dão o mesmo resultado?**
Essa câmera está em z = 5 olhando para -z com o "para cima" em y, que já é a
orientação padrão da câmera do OpenGL. Assim a parte de rotação da matriz é a
identidade e sobra só a translação de -5 em z. A matriz MODELVIEW impressa
pelo programa (teclas 1 e 2) é a mesma nos dois casos:

```
1  0  0   0
0  1  0   0
0  0  1  -5
0  0  0   1
```

Para ver o cubo de cima (tecla 6) foi usado `gluLookAt(0,5,0, 0,0,0, 0,0,-1)`.
O vup não pode ser (0,1,0) porque ficaria paralelo à direção de visão.

**1.2** - Aumentar o `fovy` abre o ângulo de visão e o cubo fica menor; diminuir
dá zoom. Um `aspect` diferente da proporção da janela estica/achata o cubo.
Com `glOrtho` não há perspectiva: as faces da frente e de trás ficam do mesmo
tamanho e o cubo aparece como um retângulo.
