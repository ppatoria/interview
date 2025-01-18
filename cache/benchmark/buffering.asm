       push   %r14
       pxor   %xmm0,%xmm0
       mov    %rdi,%r14
       push   %r13
       push   %r12
       push   %rbp
       push   %rbx
       sub    $0x620,%rsp
       mov    %rsp,%rdi
       movaps %xmm0,(%rsp)
       movq   $0x0,0x10(%rsp)
       callq  409a20 <void populateSimulatedInput<std::vector<MarketData, std::allocator<MarketData> > >(std::vector<MarketData, std::allocator<MarketData> >&)>
       cmpb   $0x0,0x1a(%r14)
       je     40912b <processUsingBufferBenchmark(benchmark::State&)+0x6b>
       mov    %r14,%rdi
       callq  40bd70 <benchmark::State::StartKeepRunning()>
       mov    %r14,%rdi
       callq  40bfa0 <benchmark::State::FinishKeepRunning()>
       mov    (%rsp),%rdi
       test   %rdi,%rdi
       je     409291 <processUsingBufferBenchmark(benchmark::State&)+0x1d1>
       mov    0x10(%rsp),%rsi
       add    $0x620,%rsp
       pop    %rbx
       pop    %rbp
       sub    %rdi,%rsi
       pop    %r12
       pop    %r13
       pop    %r14
       jmpq   405480 <operator delete(void*, unsigned long)@plt>
       mov    %r14,%rdi
       mov    0x10(%r14),%rbx
       callq  40bd70 <benchmark::State::StartKeepRunning()>
       mov    $0x40,%r12d
       lea    0x20(%rsp),%r13
       test   %rbx,%rbx
       je     4090fa <processUsingBufferBenchmark(benchmark::State&)+0x3a>
       nopw   0x0(%rax,%rax,1)
       mov    0x8(%rsp),%rax
       mov    (%rsp),%rdx
       pxor   %xmm2,%xmm2
       movabs $0xaaaaaaaaaaaaaaab,%rcx
       movapd %xmm2,%xmm1
       mov    %rax,%r8
       sub    %rdx,%r8
       mov    %r8,%r11
       sar    $0x3,%r11
       imul   %rcx,%r11
       cmp    %rdx,%rax
       je     409260 <processUsingBufferBenchmark(benchmark::State&)+0x1a0>
       pxor   %xmm2,%xmm2
       mov    %r11,%rax
       movapd %xmm2,%xmm1
       mov    $0x600,%r9d
       mov    $0x40,%r10d
       cmp    $0x3f,%rax
       jbe    409248 <processUsingBufferBenchmark(benchmark::State&)+0x188>
       mov    %r9d,%ecx
       mov    %r9d,%ebp
       mov    %r13,%rdi
       mov    %rdx,%rsi
0.16%  shr    $0x3,%ecx
       and    $0x4,%r9d
59.16% rep movsq %ds:(%rsi),%es:(%rdi)
14.76% je     4091c7 <processUsingBufferBenchmark(benchmark::State&)+0x107>
       mov    (%rsi),%ecx
       mov    %ecx,(%rdi)
       mov    $0x4,%ecx
0.21%  test   $0x2,%bpl
       je     4091db <processUsingBufferBenchmark(benchmark::State&)+0x11b>
       movzwl (%rsi,%rcx,1),%r9d
       mov    %r9w,(%rdi,%rcx,1)
       add    $0x2,%rcx
       and    $0x1,%ebp
       je     4091e8 <processUsingBufferBenchmark(benchmark::State&)+0x128>
       movzbl (%rsi,%rcx,1),%esi
       mov    %sil,(%rdi,%rcx,1)
       lea    (%r10,%r10,2),%rsi
       lea    0x28(%rsp),%rcx
       lea    0x28(%rsp,%rsi,8),%rsi
       nopw   %cs:0x0(%rax,%rax,1)
0.82%  pxor   %xmm0,%xmm0
1.17%  add    $0x18,%rcx
0.72%  cvtsi2sdl -0x10(%rcx),%xmm0
5.82%  addsd  %xmm0,%xmm1
2.31%  mulsd  -0x18(%rcx),%xmm0
0.98%  addsd  %xmm0,%xmm2
13.59% cmp    %rcx,%rsi
       jne    409200 <processUsingBufferBenchmark(benchmark::State&)+0x140>
       mov    %r12,%rcx
       add    $0x600,%rdx
       sub    $0x600,%r8
       sub    %rax,%rcx
       lea    -0x40(%rax),%rsi
       add    %r11,%rcx
       jae    409260 <processUsingBufferBenchmark(benchmark::State&)+0x1a0>
       mov    %rsi,%rax
0.29%  jmpq   409190 <processUsingBufferBenchmark(benchmark::State&)+0xd0>
       nopl   0x0(%rax)
       cmp    $0x18,%r8
       je     409279 <processUsingBufferBenchmark(benchmark::State&)+0x1b9>
       mov    %r8,%r9
       mov    %rax,%r10
       jmpq   4091a6 <processUsingBufferBenchmark(benchmark::State&)+0xe6>
       nopl   0x0(%rax)
       movq   %xmm1,%rax
       movq   %xmm2,%rax
       sub    $0x1,%rbx
       jne    409150 <processUsingBufferBenchmark(benchmark::State&)+0x90>
       jmpq   4090fa <processUsingBufferBenchmark(benchmark::State&)+0x3a>
       movdqu (%rdx),%xmm3
       mov    0x10(%rdx),%ecx
       mov    %rax,%r10
       mov    %ecx,0x30(%rsp)
       movaps %xmm3,0x20(%rsp)
       jmpq   4091e8 <processUsingBufferBenchmark(benchmark::State&)+0x128>
       add    $0x620,%rsp
       pop    %rbx
       pop    %rbp
       pop    %r12
       pop    %r13
       pop    %r14
       retq
       mov    %rax,%rbx
       jmpq   405976 <processUsingBufferBenchmark(benchmark::State&) [clone .cold]>
