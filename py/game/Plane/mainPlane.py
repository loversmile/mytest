# _*_ coding: utf-8 _*_
"""
Created on Fri Sep 5 2014

@author: jklou
"""

import pygame
from sys import exit
from pygame.locals import *
from roleClass import *
import random

# init game
pygame.init()
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption('Plane Shoot')

# load music
bullet_sound = pygame.mixer.Sound('resources/sound/bullet.wav')
enemy1_down_sound = pygame.mixer.Sound('resources/sound/enemy1_down.wav')
enemy2_down_sound = pygame.mixer.Sound('resources/sound/enemy2_down.wav')
enemy3_down_sound = pygame.mixer.Sound('resources/sound/enemy3_down.wav')
game_over_sound = pygame.mixer.Sound('resources/sound/game_over.wav')
bullet_sound.set_volume(0.3)
enemy1_down_sound.set_volume(0.3)
game_over_sound.set_volume(0.3)
pygame.mixer.music.load('resources/sound/game_music.wav')
pygame.mixer.music.play(-1,0.0)
pygame.mixer.music.set_volume(0.25)

# load background
background = pygame.image.load('resources/image/background.png').convert()
game_over = pygame.image.load('resources/image/gameover.png')

plane_image_filename = 'resources/image/plane.png'
plane_img = pygame.image.load(plane_image_filename)

# set player
player_rect = []
player_rect.append(pygame.Rect(3,101,100,124)) 
player_rect.append(pygame.Rect(168,362,100,124)) 
player_rect.append(pygame.Rect(168,236,100,124)) 
player_rect.append(pygame.Rect(333,626,100,124)) 
player_rect.append(pygame.Rect(333,500,100,124)) 
player_rect.append(pygame.Rect(435,626,100,124)) 
player_pos = [200,600]
player = Player(plane_img, player_rect, player_pos)

# set bullet
bullet_rect = pygame.Rect(1004,987,9,21)
bullet_img = plane_img.subsurface(bullet_rect)

# set enemy
enemy1_rect = pygame.Rect(536,610,53,39)
enemy1_img = plane_img.subsurface(enemy1_rect)
enemy1_down_imgs = []
enemy1_down_imgs.append(plane_img.subsurface(pygame.Rect(269,349,53,39)))
enemy1_down_imgs.append(plane_img.subsurface(pygame.Rect(875,699,53,39)))
enemy1_down_imgs.append(plane_img.subsurface(pygame.Rect(269,298,53,39)))
enemy1_down_imgs.append(plane_img.subsurface(pygame.Rect(932,699,27,39)))

enemies1 = pygame.sprite.Group()

#set enemy2
enemy2_rect = pygame.Rect(0,0,69,90)
enemy2_img = plane_img.subsurface(enemy2_rect)
enemy2_down_imgs = []
enemy2_down_imgs.append(plane_img.subsurface(pygame.Rect(430,525,69,90)))
enemy2_down_imgs.append(plane_img.subsurface(pygame.Rect(533,654,69,90)))
enemy2_down_imgs.append(plane_img.subsurface(pygame.Rect(602,654,69,90)))
enemy2_down_imgs.append(plane_img.subsurface(pygame.Rect(671,654,69,90)))
enemy2_down_imgs.append(plane_img.subsurface(pygame.Rect(740,654,69,90)))

enemies2 = pygame.sprite.Group()

#set enemy3
enemy3_rect = pygame.Rect(335,749,169,244)
enemy3_img = plane_img.subsurface(enemy3_rect)
enemy3_down_imgs = []
enemy3_down_imgs.append(plane_img.subsurface(pygame.Rect(504,749,169,244)))
enemy3_down_imgs.append(plane_img.subsurface(pygame.Rect(166,749,169,244)))
enemy3_down_imgs.append(plane_img.subsurface(pygame.Rect(0,487,169,244)))
enemy3_down_imgs.append(plane_img.subsurface(pygame.Rect(0,225,169,244)))
enemy3_down_imgs.append(plane_img.subsurface(pygame.Rect(842,749,169,244)))
enemy3_down_imgs.append(plane_img.subsurface(pygame.Rect(169,487,169,244)))
enemy3_down_imgs.append(plane_img.subsurface(pygame.Rect(673,749,169,244)))
enemy3_down_imgs.append(plane_img.subsurface(pygame.Rect(0,749,169,244)))

enemies3 = pygame.sprite.Group()

# enemies down
enemies_down = pygame.sprite.Group()
enemies_down2 = pygame.sprite.Group()
enemies_down3 = pygame.sprite.Group()

shoot_frequency = 0
enemy_frequency = 0
enemy2_frequency = 0
enemy3_frequency = 0

kill2 = 0
kill3 = 0

enemies2_down_fre = 0
enemies3_down_fre = 0

player_down_index = 16 

score = 0

clock = pygame.time.Clock()

running = True

while running:
    # set
    clock.tick(60)

    if not player.is_hit:
        if shoot_frequency % 15 == 0:
            bullet_sound.play()
            player.shoot(bullet_img)
        shoot_frequency += 1
        if shoot_frequency >= 15:
            shoot_frequency = 0

    # Create enemies
    if enemy_frequency % 50 == 0:
        enemy1_pos = [random.randint(0,SCREEN_WIDTH - enemy1_rect.width), 0]
        enemy1 = Enemy(enemy1_img, enemy1_down_imgs, enemy1_pos)
        enemies1.add(enemy1)
    enemy_frequency += 1
    if enemy_frequency >= 100:
        enemy_frequency = 0

    # Create mid enemies
    if enemy2_frequency % 150 == 149:# and enemy2_frequency > 0:
        enemy2_pos = [random.randint(0,SCREEN_WIDTH - enemy2_rect.width), 0]
        enemy2 = Enemy(enemy2_img, enemy2_down_imgs, enemy2_pos)
        enemies2.add(enemy2)
    enemy2_frequency += 1
    if enemy2_frequency >= 300:
        enemy2_frequency = 0

    # Create mid enemies
    if enemy3_frequency % 1500 == 1499:# and enemy2_frequency > 0:
        enemy3_pos = [random.randint(0,SCREEN_WIDTH - enemy3_rect.width), 0]
        enemy3 = Enemy(enemy3_img, enemy3_down_imgs, enemy3_pos)
        enemies3.add(enemy3)
    enemy3_frequency += 1
    if enemy3_frequency >= 3000:
        enemy3_frequency = 0

    # move bullet, if out screen, remove
    for bullet in player.bullets:
        bullet.move()
        if bullet.rect.bottom < 0:
            player.bullets.remove(bullet)

    # move enemies
    for enemy in enemies1:
        enemy.move()
        if pygame.sprite.collide_circle(enemy, player):
            enemies_down.add(enemy)
            enemies1.remove(enemy)
            player.is_hit = True
            game_over_sound.play()
            break
        if enemy.rect.top < 0:
            enemies1.remove(enemy)

    # move mid enemies
    for enemy in enemies2:
        enemy.move()
        if pygame.sprite.collide_circle(enemy, player):
            enemies_down2.add(enemy)
            enemies2.remove(enemy)
            player.is_hit = True
            game_over_sound.play()
            break
        if enemy.rect.top < 0:
            enemies2.remove(enemy)

    # move large enemies
    for enemy in enemies3:
        enemy.move()
        if pygame.sprite.collide_circle(enemy, player):
            enemies_down3.add(enemy)
            enemies3.remove(enemy)
            player.is_hit = True
            game_over_sound.play()
            break
        if enemy.rect.top < 0:
            enemies3.remove(enemy)

    # hit down  enemies
    enemies1_down = pygame.sprite.groupcollide(enemies1, player.bullets, 1,1)
    for enemy_down in enemies1_down:
        enemies_down.add(enemy_down)

    enemies2_down_fre += 1
    if enemies2_down_fre == 5:
        enemies2_down_fre = 0
        kill2 = 1
    enemies2_down = pygame.sprite.groupcollide(enemies2, player.bullets, kill2,1)
    kill2 = 0
    for enemy_down in enemies2_down:
            enemies_down2.add(enemy_down)

    enemies3_down_fre += 1
    if enemies3_down_fre == 10:
        enemies3_down_fre = 0
        kill3 = 1
    enemies3_down = pygame.sprite.groupcollide(enemies2, player.bullets, kill3,1)
    kill3 = 0
    for enemy_down in enemies3_down:
            enemies_down3.add(enemy_down)

    # background
    screen.fill(0)
    screen.blit(background, (0, 0))

    # draw play Plane
    if not player.is_hit:
        screen.blit(player.image[player.img_index], player.rect)
        player.img_index = shoot_frequency / 8
    else:
        player.img_index = player_down_index / 8
        screen.blit(player.image[player.img_index], player.rect)
        player_down_index += 1
        if player_down_index > 47:
            running = False

    # draw hit enemies 
    for enemy_down in enemies_down:
        if enemy_down.down_index == 0:
            enemy1_down_sound.play()
        if enemy_down.down_index > 7:
            enemies_down.remove(enemy_down)
            score += 1000
            continue
        screen.blit(enemy_down.down_imgs[enemy_down.down_index / 2], enemy_down.rect)
        enemy_down.down_index += 1

    for enemy_down in enemies_down2:
        if enemy_down.down_index == 0:
            enemy2_down_sound.play()
        if enemy_down.down_index > 9:
            enemies_down.remove(enemy_down)
            score += 2000
            continue
        screen.blit(enemy_down.down_imgs[enemy_down.down_index / 2], enemy_down.rect)
        enemy_down.down_index += 1

    for enemy_down in enemies_down3:
        if enemy_down.down_index == 0:
            enemy3_down_sound.play()
        if enemy_down.down_index > 15:
            enemies_down.remove(enemy_down)
            score += 30000
            continue
        screen.blit(enemy_down.down_imgs[enemy_down.down_index / 2], enemy_down.rect)
        enemy_down.down_index += 1

    # draw bullet and enemy
    player.bullets.draw(screen)
    enemies1.draw(screen)
    enemies2.draw(screen)
    enemies3.draw(screen)
    
    # draw score
    score_font = pygame.font.Font(None, 36)
    score_text = score_font.render(str(score), True, (128,128,128))
    text_rect = score_text.get_rect()
    text_rect.topleft = [10, 10]
    screen.blit(score_text, text_rect)

    # update screen
    pygame.display.update()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            exit()

    # listen event
    key_pressed = pygame.key.get_pressed()
    if not player.is_hit:
        if key_pressed[K_w] or key_pressed[K_UP]:
            player.moveUp()
        if key_pressed[K_s] or key_pressed[K_DOWN]:
            player.moveDown()
        if key_pressed[K_a] or key_pressed[K_LEFT]:
            player.moveLeft()
        if key_pressed[K_d] or key_pressed[K_RIGHT]:
            player.moveRight()

font = pygame.font.Font(None, 48)
text = font.render('Score: ' + str(score), True, (255, 0, 0))
text_rect = text.get_rect()
text_rect.centerx = screen.get_rect().centerx
text_rect.centery = screen.get_rect().centery + 24
screen.blit(game_over, (0, 0))
screen.blit(text, text_rect)

while 1:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            exit()
    pygame.display.update()


           




