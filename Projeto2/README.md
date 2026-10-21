# Aula prática 03 - Transformações e Visibilidade 3D

| Arquivo | Exercício |
|---|---|
| `cube_vis.c` | 1 - oclusão com dois cubos (vermelho e azul), sem z-buffer |
| `robot_vis.c` | 2 - cópia do `robot.c` original com shoulder/elbow sólidos e z-buffer |
| `robot_pega.c` | 3 - braço da aula 02 pegando um objeto e colocando numa caixa |

`cube_vis.c` e `robot_vis.c` partem do `cube.c` e do `robot.c` do pacote
`redbook.zip`; `robot_pega.c` parte do braço feito no `Projeto1/robot.c`.
As teclas de cada programa estão no comentário do início do arquivo.

## Compilar

**Windows (MSYS2):** com o freeglut instalado (`pacman -S mingw-w64-x86_64-freeglut`
no terminal MSYS2 MINGW64), rode `compilar.bat` nesta pasta. Ele gera os
`.exe` e copia a `libfreeglut.dll` para cá, então dá para abrir com dois cliques.

**Linux** (comando do roteiro):

```
g++ robot_pega.c -o robot_pega -lGL -lGLU -lglut && ./robot_pega
```

## 1 - Oclusão (`cube_vis.c`)

Teclas: `s`/`S` movem o cubo azul para direita/esquerda, `i` inverte a ordem
de desenho.

O cubo vermelho está em z = 0 e o azul em z = -3 (mais longe). **A imagem não
fica coerente**: o azul é desenhado depois e aparece por cima do vermelho,
mesmo estando atrás dele (Figura 1a). Sem o z-buffer, o OpenGL só pinta os
pixels na ordem em que os objetos chegam, sem comparar a profundidade.

Com `i` o azul passa a ser desenhado primeiro e o vermelho pinta por cima,
então a imagem fica correta (Figura 1b). Isso só funciona porque a ordem foi
acertada "na mão" (algoritmo do pintor); se os objetos se cruzarem, nenhuma
ordem resolve.

## 2 - Oclusão no robot (`robot_vis.c`)

Teclas: `s`/`S` shoulder, `e`/`E` elbow, `z` liga/desliga o z-buffer,
`a`/`A` aproxima/afasta a câmera, `c`/`C` sobe/desce a câmera.

Mudanças em relação ao `robot.c`: shoulder vermelho e elbow azul com
`glutSolidCube`, o elbow gira em `y` e a junta do elbow foi movida para dentro
do shoulder (`glTranslatef(0.5, 0, 0)` no lugar de `1.0`). O z-buffer está
ligado como o roteiro pede (`GLUT_DEPTH`, `glEnable(GL_DEPTH_TEST)` e
`GL_DEPTH_BUFFER_BIT` no `glClear`). A tecla `z` serve só para comparar.

- **Sem z-buffer:** o elbow (desenhado por último) aparece inteiro por cima
  do shoulder, inclusive a parte que está dentro dele.
- **Com z-buffer:** a parte do elbow dentro do shoulder fica escondida e, ao
  girar o elbow, cada um aparece na frente onde realmente está mais perto.
- **Z-fighting:** shoulder e elbow têm a mesma altura (0.4) e, com o elbow em
  0°, a mesma profundidade (1.0). Por isso as faces de cima, de baixo e da
  frente ficam no mesmo plano na região sobreposta, e os dois cubos ficam
  "brigando" pelo mesmo pixel. A região pisca ou mostra listras vermelhas/azuis ao
  aproximar e subir a câmera (`a`, `c`) e girar o shoulder (`s`).

## 3 - Pegando objetos (`robot_pega.c`)

Teclas do braço iguais às da aula 02 (`b`, `s`, `e`, `w`, `t`, `f`) mais
`p` pega/solta e `r` recomeça.

Depois de desenhar os dedos, o código anda até o ponto entre eles e guarda a
matriz atual (`glGetFloatv(GL_MODELVIEW_MATRIX, ...)`). Enquanto o objeto está
preso, ele é desenhado ali, dentro da pilha de transformações do braço, e por
isso acompanha a base, o ombro, o cotovelo e o punho. Quando é solto, essa
matriz é guardada e o objeto passa a ser desenhado com `glLoadMatrixf`, ficando
parado onde a garra o deixou. Só dá para pegar se a garra estiver perto do
objeto (a distância aparece no terminal).

Sequência que funciona a partir da posição inicial:

1. `S` 3x, `E` 12x, `W` 3x - a garra desce até o cubo roxo; `p` pega;
2. `e` 6x - levanta o braço;
3. `B` 18x - gira a base 90° até a caixa;
4. `E` 6x - abaixa dentro da caixa; `p` solta.

Limitações: não há física. Se soltar no ar, o objeto fica flutuando, e as
peças podem atravessar umas às outras. Ao pegar, o objeto assume a rotação da
garra.
