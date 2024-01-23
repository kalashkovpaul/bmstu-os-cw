# Operating systems course work (with paper), BMSTU IU7 (ICS7), VII semester, 2023

## Linux loadable kernel module (LVM) for file protection agains attacks from privileged users, editing, writing, changing file owner user and group, rename and remove operations

- [x] Choose topic
- [x] Start TEX paper
- [x] Write... well, kernel module
- [x] Write... something in the paper
- [x] Pass paper submition - WITH TROUBLE

В назидание тем студентам ИУ7, которые найдут эту работу: она была сдана Ю. В. Строганову, оценка 4. У Натальи Юрьевны возникли проблемы с этой работой, в том виде, в котором она выложена сейчас, я не рекомендую её сдавать в ближайшие годы (2023+). НО эту работу можно улучшить, причём так, что даже Н. Ю. одобрит.

Во-первых, необходимо добавить минимум один подраздел в аналитическую часть, поясняющий, что заменять file_operations так, как сделано в этой работе, можно (желательно, с хоть каким-нибудь доказательствами, что будут вызываться новые функции, а не старые - можно попробовать привести листинг кода ядра, например, код open, см. лабу за основной курс, где вызывается функция, лежащая в inode->f_ops).

Во-вторых, можно добавить ещё один подраздел в аналитическую часть, сравнивающий два метода, которыми можно достигнуть поставленную задачу: этот и метод, связанный с хуками, зарегистрированными в таблице системных вызовов. Мой метод короче, проще и очевиднее. Пример того, как поставленная задача может быть решена (почти решена, без предотвращения операций смены прав), можно посмотреть по [ссылке](https://github.com/oxqo-hk/file_protection/blob/master/syscall_hook.c).

В-третьих, можно увеличить функционал программы (это делается довольно просто) в двух местах:
1. Обеспечение сохранения возможности изменения файла выбранным процессом/процессами. Выполняется посредством небольшого изменения функций-хуков (опять же, см. ссылку выше).
2. Обеспечение защиты нескольких файлов с возможностью изменить набор защищаемых файлов без выгрузки модуля. Выполняется посредством добавления передачи информации из режима пользователя в режим ядра (см. лабу за основной курс, например, фортунки).

В-четвёртых, у человека, который прислушался к предыдущим трём советам, есть все шансы хорошо сдать работу (даже Н. Ю.), т.к. он: не я (у Н.Ю. проблемы конкретно со мной), его программа будет обоснованной даже для Н.Ю. и его программа будет больше чем 140 строк и иметь передачу данных между режимом ядра и режимом пользователя.

По всем вопросам можно писать в личку, удачи!