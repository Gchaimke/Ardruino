# Ardruino
## ardruino modules to control cleaner-robot
- [x] Finish my changes
- [X] Push my commits to GitHub
- [X] lets check
- [X] Add examples to module
- [X] Add TEST/Voltmeter
- [ ] Motor calibration
- [ ] Start and stop slowly
- [ ] При нахождении препятствий, остановись, рассчитай разницу дальности, поверни туда где дальше.
	  Например: левый 10, правый 12 - поверни немного влево. Если большая разница - приблизься к
	  самому ближнему и обойди.
- [ ] Заказать оставшиеся датчики (чип времени, инфракрасный, компас?! GPS ?! ) 


## 12/07/17  	Yossi
```
	Я проверил Скетч двигателей, всё работает как надо. ничего не надо менять.
	Можно продолжать дальше...
```
## 11/07/17  	Yossi
```
	Надо подобрать аркамулятор от ноутбука, который можно заряжать и проверять вольтаж
	через аналоговый вход на андруино. Таким образом робот будет определять заряд и 
	останавливаться чтобы не испортить батарейки. Также, позже можно будет сделать станцию
	для зарядки и маяк. как только будет мало заряда, то пусть возвращается...
```
## 11/07/17  	Yossi
```
	Added Ultrasonic serial port check. 
	Sorted Libraries and folders.
```

## 10/07/17  	Chaim Gorbov
```
	Added tow examples to check motor module.
```

## 10/07/17 	Chaim Gorbov
```
	added new module - L298NDualMotor.
	Now to compile .ino file, module folder must
	to be in "C:\Users\user\Documents\Arduino\libraries\"
	.ino file fixed. lets check?
```

## 09/07/17		Yossi Gorbov
 Закончил заниматься с двигателем. Тянет в право. Начинаем с замедлением И разработкой поворотов.

## 08/07/17 	Yossi Gorbov
 Ты был прав. Есть ещё две ножки для скорости. ena enb. Но там по два пина . И что с ними делать ?! Проснешься, позвони мне...
