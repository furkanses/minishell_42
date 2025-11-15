# minishell

Basitleştirilmiş bir **bash** klonudur. Kullanıcıdan komut satırı alır, bu satırı tokenize eder, environment değişkenlerini genişletir, soyut bir sözdizim ağacına (AST) çevirir ve sonunda bu ağacı yürüterek komutları çalıştırır. :contentReference[oaicite:0]{index=0}

---

## Özellikler

- Prompt gösterme ve `readline` ile kullanıcı girdisi alma  
- History desteği  
- `PATH` üzerinden binary bulma  
- Tek global değişkenle sinyal yönetimi (`SIGINT`, `SIGQUIT`, `Ctrl-D`)  
- Tırnak kuralları:
  - '...' → hiçbir genişleme olmaz  
  - "..." → sadece `$` genişler  
- Redirection’lar: <, >, >>, << (heredoc)  
- Pipe desteği: cmd1 | cmd2 | cmd3  
- Environment değişkenleri: `$VAR`, `$?`  
- Builtin komutlar: echo, cd, pwd, export, unset, env, exit  

---

# Proje Yapısı

minishell/  
  builtins/  
  lexer/  
  expander/  
  parser/  
  execute/  
  utils/  
  libft/  
  ft_printf/  
  minishell.c  
  Makefile  

---

# Çalışma Akışı

1. readline → input al  
2. lexer → token listesi oluştur  
3. expander → `$VAR`, `$?`, tırnak genişlemeleri  
4. parser → token'lardan AST oluştur  
5. executor → fork/execve/pipe/redirection/builtin çalıştır  
6. Exit status saklanır → prompt tekrar gösterilir  

**Kısaca:**  
readline → lexer → expander → parser → executor → exit status → prompt

---

# Lexer

Lexer’ın görevi kullanıcı girdisini **token**lara ayırmaktır.

### Görevleri
- Kelime token’ları oluşturma  
- Operatörleri ayırma (|, <, >, >>, <<)  
- Tırnak bloklarını tek token olarak alma  
- Boşluk atlama  
- Token türlerini işaretleme  

Lexer sadece AYIRIR, genişletme yapmaz.

---

# Expander

Expander şu genişletmeleri yapar:

- `$VAR` → environment değeri  
- `$?` → son komut status’ü  
- Tek tırnak → hiçbir genişleme yok  
- Çift tırnak → `$` genişler  

Genişlemelerden sonra token içeriği son halini alır.

---

# Parser

Parser token listesini **AST (Abstract Syntax Tree)** yapısına çevirir.

### Node türleri:
- PIPE  
- CMD  
- REDIR_IN  
- REDIR_OUT  
- REDIR_APPEND  
- HEREDOC  

Parser, hangi komutun hangi redirection’a sahip olduğunu ve pipe zincirini kurar.

---

# Executor

AST yürütülür:

- fork()  
- execve()  
- pipe(), dup2()  
- open(), close()  
- builtins çalıştırma  
- heredoc yönetimi  

### Pipe mantığı
- pipe() oluşturulur  
- Sol komut → stdout → pipe write  
- Sağ komut → stdin → pipe read  
- Parent → fd’leri kapatır, çocukları bekler  

### Builtin davranışı
- Pipe yoksa → parent’ta çalışır  
- Pipe içindeyse → child’da çalışır  

Binary bulunamazsa:  
`command not found` → exit code 127

---

# Sinyaller (Signals)

Minishell sinyalleri bash gibi yönetir.  
Tek global değişken (ör. g_status) kullanılır.

---

## SIGINT (Ctrl-C)

### Prompt modunda:
- Mevcut satırı keser  
- Yeni prompt gösterir  
- Exit code → 130  
- SIGQUIT ignore edilir  

### Child sırasında:
- Child ölür  
- Parent sadece exit status alır  

---

## SIGQUIT (Ctrl-\)

### Prompt modunda:
- YOK SAYILIR, ekran çıktısı olmaz  

### Child sırasında:
- Varsayılan davranış  
- Exit status 131 olabilir  

---

## EOF (Ctrl-D)

### Prompt boştayken:
- Shell kapanır (exit davranışı)

### Prompt doluyken:
- readline satırı sonlandırır  

---

## Teknik Ayarlar

Prompt sırasında:  
signal(SIGINT, sigint_handler)  
signal(SIGQUIT, SIG_IGN)

Child process’de:  
signal(SIGINT, SIG_DFL)  
signal(SIGQUIT, SIG_DFL)

---

# Derleme ve Çalıştırma

make  
./minishell  

---

# Örnek Kullanım

minishell$ export NAME=ali  
minishell$ echo "hello $NAME"  
hello ali  

minishell$ ls | grep minishell > out.txt  

---

# Lisans

Bu proje, 42 okulu minishell konusu baz alınarak geliştirilmiştir.
