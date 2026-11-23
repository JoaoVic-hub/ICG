# Aula prática 05 - Mapeamento de Texturas

| Arquivo | Exercício |
|---|---|
| `checker_textura.c` | 1 - geração de texturas: um quadrado só, 0x8/0x1, NEAREST/LINEAR, escala 10, REPEAT/CLAMP |
| `textura_imagem.c` | 2 - carregar imagem com `stb_image.h`, cor vermelha, modos de `glTexEnv` e esfera |

Os dois partem do `checker.c` do pacote `redbook.zip`.
`stb_image.h` (v2.34, domínio público, de github.com/nothings/stb) e
`terra.jpg` (mapa da Terra da NASA, domínio público, reduzido para 1024 x 512)
ficam nesta pasta.
As teclas de cada programa estão na seção de cada exercício abaixo.

## Compilar

**Windows (MSYS2):** com o freeglut instalado (`pacman -S mingw-w64-x86_64-freeglut`
no terminal MSYS2 MINGW64), rode `compilar.bat` nesta pasta.

**Linux** (comando do roteiro):

```
g++ textura_imagem.c -o textura_imagem -lGL -lGLU -lglut && ./textura_imagem
```

## 1 - Geração de texturas (`checker_textura.c`)

Teclas: `q` um quadrado só, `x` alterna o hexadecimal 0x8/0x1, `l` alterna
`GL_NEAREST`/`GL_LINEAR`, `e` alterna a escala 1/10 e `c` alterna
`GL_REPEAT`/`GL_CLAMP`. O estado atual aparece no título da janela.

Ordem do roteiro: abrir, `q`, `x`, `l`, `x`, `e`, `c`.

- **Original:** começa igual ao `checker.c`, com os dois quadrados.
- **Um quadrado só (`q`):** o segundo quadrado sai. Os vértices de cima do
  primeiro vão para o fundo e para cima (`(±2, 4, -6)`), e os de baixo vêm
  para perto da câmera (`(±2, -1, 2)`). Fica um piso em perspectiva, como a
  figura 1.
- **0x8 para 0x1 (`x`):** o teste `i & 0x8` troca de cor a cada 8 texels.
  Com `0x1` troca a cada texel, então a textura de 64 x 64 vira um tabuleiro
  de 64 x 64 casas. Com `GL_NEAREST` o fundo do piso fica serrilhado, com
  padrões de moiré.
- **`GL_LINEAR` (`l`):** em vez de pegar o texel mais próximo, o OpenGL faz a
  média dos 4 texels vizinhos. Com casas de 1 texel isso mistura preto e
  branco, e o piso fica cinza e borrado. O moiré diminui mas não some, porque
  não há mipmap. De volta a 0x8 (`x`), as casas grandes de perto ficam com as
  bordas borradas.
- **Escala 10 (`e`):** as coordenadas vão de 0 a 10. Com `GL_REPEAT` a
  textura se repete 10 vezes em cada direção (100 cópias).
- **`GL_CLAMP` (`c`):** as coordenadas acima de 1 são presas em 1. A textura
  aparece uma vez só, no canto `(0, 0)`-`(1, 1)`, que é 1/10 do quadrado e fica
  fora da tela embaixo à esquerda. O resto repete os texels da borda,
  esticados, e vira faixas. Com `GL_LINEAR` as faixas ficam cinza, porque o
  `GL_CLAMP` mistura a borda com a cor de borda (preta).

## 2 - Carregando imagens de textura (`textura_imagem.c`)

Teclas: `v` cor branca/vermelha, `m` próximo modo, `1` a `5` escolhem o modo
(`GL_DECAL`, `GL_REPLACE`, `GL_MODULATE`, `GL_BLEND`, `GL_ADD`) e `e`
alterna quadrado/esfera. O estado atual aparece no título da janela.

Ordem do roteiro: abrir, `v`, `2`, `3`, `4`, `5`, depois `e` e de novo `1` a `5`.

O `loadTexture()` do roteiro carrega o `terra.jpg` no começo do `display()`.
O quadrado vai de `(-2, -1)` a `(2, 1)` para manter a proporção 2:1 da imagem.
A esfera é a do roteiro (`gluSphere(quad, 1.0, 32, 32)`), girada para o polo
norte ficar para cima e a América do Sul de frente.

Resultados com a cor vermelha `(1, 0, 0)` (`Cf` = cor do objeto, `Ct` = cor
da textura):

| Modo | Conta | O que aparece |
|---|---|---|
| `GL_DECAL` | `Ct` (imagem RGB, sem alfa) | só a imagem; o vermelho não muda nada |
| `GL_REPLACE` | `Ct` | igual ao DECAL |
| `GL_MODULATE` | `Cf · Ct` | só o canal vermelho da imagem; oceano quase preto, deserto e gelo vermelhos |
| `GL_BLEND` | `Cf · (1 - Ct) + Cc · Ct`, com `Cc` = preto | negativo em vermelho: o escuro (oceano) fica vermelho e o claro (gelo) fica preto |
| `GL_ADD` | `Cf + Ct` | imagem clara e rosada; o vermelho satura e o gelo fica branco |

Na esfera os resultados são os mesmos. O `gluQuadricTexture(quad, GL_TRUE)`
faz a GLU gerar as coordenadas de textura, e o mapa da Terra fecha certinho em
volta dela.

### Correções no código do roteiro

- O Listing 1 usa `std::cerr`, que é C++. Em C troquei por
  `fprintf(stderr, ...)`.
- A variável `texture` não é declarada no roteiro. Declarei
  `static GLuint texture;`.
- Chamar `loadTexture()` no `display()` a cada quadro relê o arquivo e cria
  uma textura nova a cada redesenho (vazamento de memória). Coloquei
  `if (texture == 0)` antes, para carregar só uma vez.
- O `stb_image` entrega a imagem de cima para baixo, e no OpenGL `t = 0` é
  embaixo. Sem correção a imagem fica de cabeça para baixo. Usei
  `stbi_set_flip_vertically_on_load(1)`.
- O `gluNewQuadric()` também é criado uma vez só, no `init()`, e não a cada
  quadro.
