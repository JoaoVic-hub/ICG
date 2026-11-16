# Aula prática 04 - Iluminação e Sombreamento

| Arquivo | Exercício |
|---|---|
| `light_fontes.c` | 1 - luz 0 vermelha e posicional + lanterna verde à esquerda |
| `movelight_trans.c` | 2 - luz transladando e passando por dentro do toroide |
| `movelight_camera.c` | 2 - dois toroides e luz presa na câmera |
| `light_shade.c` | 3 - FLAT x SMOOTH, slices, stacks e tempo de renderização |
| `light_rgb.c` | 4 - três luzes (vermelha, verde, azul) girando em torno da esfera |

Todos partem do `light.c` e do `movelight.c` do pacote `redbook.zip`.
As teclas de cada programa estão na seção de cada exercício abaixo.

## Compilar

**Windows (MSYS2):** com o freeglut instalado (`pacman -S mingw-w64-x86_64-freeglut`
no terminal MSYS2 MINGW64), rode `compilar.bat` nesta pasta.

**Linux** (comando do roteiro):

```
g++ light_rgb.c -o light_rgb -lGL -lGLU -lglut && ./light_rgb
```

## 1 - Fontes de luz (`light_fontes.c`)

Teclas: `a`, `d`, `e` ligam/desligam as componentes ambiente, difusa e
especular da luz 0. `c` alterna branca/vermelha, `p` alterna
direcional/posicional e `1` liga/desliga a lanterna.

- **Luz 0:** começa igual ao `light.c` original, branca e direcional
  (`(1, 1, 1, 0)`). Com `c` e `p` ela fica vermelha e posicional
  (`(1, 1, 1, 1)`). Com `w = 1` a luz é pontual. Com `w = 0` ela é
  direcional, e `(1, 1, 1)` é só a direção.
- **Luz 1:** lanterna verde em `(-2, 0, 1)`, apontando para a esfera
  (`GL_SPOT_DIRECTION = (1, 0, -0.5)`), com corte de 30° e atenuação.

A componente difusa dá a cor da luz no objeto. A especular dá a cor do
brilho. A ambiente clareia a esfera inteira por igual, até o lado sem luz.

## 2 - Movendo fontes de luz

**`movelight_trans.c`:** começa igual ao `movelight.c` original, com a luz
girando 30° a cada clique esquerdo. A tecla `m` troca o `glRotated` por
`glTranslated(0, 0, lightZ)`. Nesse modo, o botão esquerdo do mouse leva a
luz para `-z` e o direito para `+z`. Ela atravessa o furo do toroide, e o lado
iluminado muda de face. A câmera foi posta em `(4, 2, 4)` para dar para ver a
passagem.

**`movelight_camera.c`:** o `glLightfv(GL_LIGHT0, GL_POSITION, ...)` com
`(0, 0, 0, 1)` vem antes do `gluLookAt`, com a modelview ainda identidade.
Assim a luz fica na posição do olho e anda junto com a câmera
(`w`/`s`, `a`/`d`, `q`/`e`). O segundo toroide está em `z = -3`.

## 3 - Sombreamento (`light_shade.c`)

Teclas: `f` FLAT/SMOOTH, `t`/`T` dobram/dividem stacks, `s`/`S` dobram/dividem
slices. Começa em SMOOTH, 20 x 16, como o `light.c`. O tempo de cada quadro
aparece no título da janela e no terminal.

- **FLAT:** cada polígono recebe uma cor só, e as facetas ficam visíveis.
  Aumentar só os stacks não basta, porque as faixas dos slices continuam
  aparecendo. A imagem só ficou lisa com cerca de **160 slices x 1024 stacks**.
- **SMOOTH:** a cor é calculada nos vértices e interpolada. Com
  **40 slices x 32 stacks** a esfera já fica lisa.
- **Tempo:** a esfera tem cerca de `2 x slices x stacks` triângulos. Na minha
  máquina, até 20 x 4096 tudo ficou abaixo de 3 ms. Com 320 x 4096 (2,6
  milhões de triângulos), um quadro levou 20 a 40 ms, tanto em FLAT quanto em
  SMOOTH. O custo vem do número de polígonos, não do modelo de sombreamento.
  Por isso o SMOOTH com poucos polígonos é bem mais barato que o FLAT com
  muitos.

## 4 - Várias fontes de luz (`light_rgb.c`)

Teclas: `r`, `g`, `b` ligam/desligam cada luz. `espaço` ou o botão esquerdo do
mouse giram as três luzes em 30°.

| Luz | Cor | Posição inicial | Gira em torno de |
|---|---|---|---|
| `GL_LIGHT0` | vermelha | `(0, 1.5, 1.5)` em cima | x |
| `GL_LIGHT1` | verde | `(1.5, 0, 1.5)` à direita | y |
| `GL_LIGHT2` | azul | `(-1.5, 0, 1.5)` à esquerda | z |

O `display()` e o `reshape()` vêm do `movelight.c`. Cada luz tem seu próprio
`glPushMatrix`/`glRotated`/`glPopMatrix`, e por isso o mesmo ângulo `spin`
gira cada uma em um eixo diferente. Os cubinhos coloridos mostram onde está
cada luz.
