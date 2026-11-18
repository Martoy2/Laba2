# Команды

## Массив (M)

| Команда | Что делает |
|---|---|
| `MPUSH arr x` | добавляет в конец |
| `MINSERT arr 2 x` | вставляет по индексу |
| `MGET arr 2` | показывает по индексу |
| `MSET arr 2 x` | заменяет по индексу |
| `MDEL arr 2` | удаляет по индексу |
| `MLEN arr` | длина |

## Односвязный список (F)

| Команда | Что делает |
|---|---|
| `FPUSH list x` | в хвост |
| `FPUSHHEAD list x` | в голову |
| `FPUSHTAIL list x` | в хвост |
| `FPUSHBEFORE list a x` | перед `a` |
| `FPUSHAFTER list a x` | после `a` |
| `FDELHEAD list` | удаляет голову |
| `FDELTAIL list` | удаляет хвост |
| `FDELBEFORE list a` | удаляет элемент перед `a` |
| `FDELAFTER list a` | удаляет элемент после `a` |
| `FDEL list x` | удаляет `x` |
| `FFIND list x` | ищет `x` |
| `FGET list` | выводит с головы |
| `FGETREV list` | выводит с хвоста |
| `FLEN list` | длина |

## Двусвязный список (L)

Те же команды, что у `F`: `LPUSH`, `LPUSHHEAD`, `LPUSHTAIL`, `LPUSHBEFORE`, `LPUSHAFTER`, `LDELHEAD`, `LDELTAIL`, `LDELBEFORE`, `LDELAFTER`, `LDEL`, `LFIND`, `LGET`, `LGETREV`, `LLEN`.

## Стек (S)

| Команда | Что делает |
|---|---|
| `SPUSH st x` | кладёт наверх |
| `SPOP st` | снимает верхний |
| `SPEEK st` | показывает верхний |
| `SLEN st` | размер |

## Очередь (Q)

| Команда | Что делает |
|---|---|
| `QPUSH q x` | в конец |
| `QPOP q` | забирает первый |
| `QPEEK q` | показывает первый |
| `QLEN q` | размер |

## АВЛ-дерево (T)

| Команда | Что делает |
|---|---|
| `TINSERT tree x` | вставляет |
| `TDEL tree x` | удаляет |
| `TFIND tree x` | ищет `x` |
| `ISMEMBER tree x` | ищет `x` |
| `TGET tree` | по возрастанию |
| `TGET tree preorder` | обход `preorder`, `postorder` или `bfs` |
| `THEIGHT tree` | высота |
| `TLEN tree` | размер |

## Общее

| Команда | Что делает |
|---|---|
| `PRINT name` | выводит любую структуру |
